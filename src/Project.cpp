#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <Syslog.h>
#include <base64.h>
#include <ArduinoJson.h>
#include <secrets.h>

#define HOSTNAME "RobotPotager"
// Scheduler variables
#define _TASK_SLEEP_ON_IDLE_RUN

// Wifi variables
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

// Domoticz variables
int WeMos_dz_Hw_idx = 0;
int WeMos_dz_Plan_idx = 0;

const char* dz_signal_name = "Reseau";
int dz_signal_idx = 0;
// int dz_signal_val = 0;

// Syslog server connection info
#define SYSLOG_SERVER "192.168.1.100"
#define SYSLOG_PORT 10500

// Prototype
void print_WifiSignal();
void get_domoticzIdx();
void send_signal();

// Timer/Scheduler
Scheduler runner;

// List tasks
// Task t1(2*60*1000, TASK_FOREVER, &print_WifiSignal, &runner, true);    //adding task to the chain on creation
Task update_dz_idx(4*60*60*1000, TASK_FOREVER, &get_domoticzIdx, &runner, true);  //Get Idx every 4 hours
Task update_dz_signal(15*60*1000, TASK_FOREVER, &send_signal, &runner, true);  //Update Wifi signal every 15mn

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, HOSTNAME, LOG_KERN);

//Object of class HTTPClient
HTTPClient http;

void write_output(String message){
  syslog.log(LOG_INFO, message);
  Serial.println(message);
}

int get_WifiSignal() {
    int signal = WiFi.RSSI();
    char message[64];
    sprintf(message, "Wifi signal: %d dB", signal);
    write_output(message);
    return signal;
}

void send_signal(){
//  /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=VALUE
   String URL = DZ_API + String("type=command&param=udevice&idx=")
                       + String(dz_signal_idx)
                       + String("&nvalue=0&svalue=")
                       + String(get_WifiSignal());
  WiFiClient client;
  http.begin(client, URL);
  // write_output(URL);
  String auth = base64::encode(String(DZ_USER) + ":" + String(DZ_PASS));
  // write_output(auth);
  http.addHeader("Authorization", "Basic " + auth);

  int httpCode = http.GET();
  String payload;
  //Check the returning code
  if (httpCode != 200) {
    write_output("HTTP call in send_signal error");
    write_output(String(httpCode));
  }
  // else{
  //   payload = http.getString();
  //   write_output(payload);
  // }
}

void get_HardwareIdx(){
  // JSON document
  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = 5144;
  DynamicJsonDocument dzHardware(capacity);

  String URL = DZ_API + String("type=hardware");
  WiFiClient client;
  http.begin(client, URL);
  // write_output(URL);
  String auth = base64::encode(String(DZ_USER) + ":" + String(DZ_PASS));
  // write_output(auth);
  http.addHeader("Authorization", "Basic " + auth);

  int httpCode = http.GET();
  String payload;
  //Check the returning code
  if (httpCode != 200) {
    write_output("HTTP call in get_domoticzIdx error");
    write_output(String(httpCode));
  }else{
    payload = http.getString();
    // write_output(payload);
  }
  http.end();   //Close connection

  // Parse JSON object
  // Only keep few fields
  StaticJsonDocument<200> filter;
  filter["result"][0]["Name"] = true;
  filter["result"][0]["idx"] = true;
  filter["result"][0]["Enabled"] = true;

  DeserializationError error = deserializeJson(dzHardware, payload, DeserializationOption::Filter(filter));

  if (error) {
    write_output(F("deserializeJson() failed: "));
    write_output(error.c_str());
    return;
  }

  // Extract values
  // Serial.println(F("Response:"));
  // serializeJson(dzHardware["result"], Serial);
  // extract the values
  // Serial.println(F("JsonArry:"));

  // Retreive hardware
  JsonArray array = dzHardware["result"].as<JsonArray>();
  for(JsonVariant v : array) {
    if(String(HOSTNAME).equals(v["Name"].as<char*>())){
      WeMos_dz_Hw_idx = v["idx"].as<int>();
    }
  }
}

void get_PlanIdx(){
  // JSON document
  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = 512;
  DynamicJsonDocument dzReturn(capacity);

  String URL = DZ_API + String("type=plans");
  WiFiClient client;
  http.begin(client, URL);
  // write_output(URL);
  String auth = base64::encode(String(DZ_USER) + ":" + String(DZ_PASS));
  // write_output(auth);
  http.addHeader("Authorization", "Basic " + auth);

  int httpCode = http.GET();
  String payload;
  //Check the returning code
  if (httpCode != 200) {
    write_output("HTTP call in get_domoticzIdx error");
    write_output(String(httpCode));
  }
  // else{
  //   payload = http.getString();
  //   // write_output(payload);
  // }

  // Parse JSON object
  // Only keep few fields
  StaticJsonDocument<200> filter;
  filter["result"][0]["Name"] = true;
  filter["result"][0]["idx"] = true;

  DeserializationError error = deserializeJson(dzReturn, http.getStream(), DeserializationOption::Filter(filter));

  http.end();   //Close connection
  if (error) {
    write_output(F("deserializeJson() failed: "));
    write_output(error.c_str());
    return;
  }

  // Extract values
  // Serial.println(F("Response:"));
  // serializeJson(dzHardware["result"], Serial);
  // extract the values
  // Serial.println(F("JsonArry:"));

  // Retreive hardware
  JsonArray array = dzReturn["result"].as<JsonArray>();
  for(JsonVariant v : array) {
    if(String(HOSTNAME).equals(v["Name"].as<char*>())){
      WeMos_dz_Plan_idx = v["idx"].as<int>();
    }
  }
}

void get_DevicesIdx(){
  // JSON document
  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = 10000;
  DynamicJsonDocument dzDevice(capacity);

  String URL = DZ_API + String("type=devices&plan=")+String(WeMos_dz_Plan_idx);
  WiFiClient client;
  http.begin(client, URL);
  // write_output(URL);
  String auth = base64::encode(String(DZ_USER) + ":" + String(DZ_PASS));
  // write_output(auth);
  http.addHeader("Authorization", "Basic " + auth);

  int httpCode = http.GET();
  // char* payload;
  //Check the returning code
  if (httpCode != 200) {
    write_output("HTTP call in get_domoticzIdx error:");
    write_output(String(httpCode));
    return;
  }
  // else{
  //   write_output("[HTTP] GET code:");
  //   write_output(String(httpCode));
    // payload = http.getString();
    // write_output(payload);
  // }

  // Parse JSON object
  // Only keep few fields
  StaticJsonDocument<300> filter;
  // filter["result"][0]["PlanId"] = true;
  filter["result"][0]["Type"] = true;
  filter["result"][0]["SubType"] = true;
  filter["result"][0]["SwitchType"] = true;
  filter["result"][0]["Name"] = true;
  filter["result"][0]["Data"] = true;
  filter["result"][0]["idx"] = true;

  // serializeJson(filter, Serial);

  DeserializationError error = deserializeJson(dzDevice, http.getStream(), DeserializationOption::Filter(filter));

  if (error) {
    write_output(F("deserializeJson() failed: "));
    write_output(error.c_str());
    return;
  }

  http.end();   //Close connection
  // Extract values
  Serial.println(F("Response:"));
  serializeJson(dzDevice, Serial);
  // extract the values
  // Serial.println(F("JsonArry:"));

  // Retreive devices
  JsonArray array = dzDevice["result"].as<JsonArray>();
  for(JsonVariant v : array) {
    if(String(dz_signal_name).equals(v["Name"].as<char*>())){
      dz_signal_idx = v["idx"].as<int>();
    }
  }
}

void get_domoticzIdx(){
  // Get hardware IDX from Domoticz
  get_HardwareIdx();
  // write_output(F("WeMos_dz_Hw_idx:"));
  // write_output(String(WeMos_dz_Hw_idx));

  // Get plan IDX from Domoticz
  get_PlanIdx();
  // write_output(F("WeMos_dz_Plan_idx:"));
  // write_output(String(WeMos_dz_Plan_idx));

  // Get devices IDX from Domoticz
  get_DevicesIdx();
}

void setup() {
  // Region WiFi
  Serial.begin(115200);
  Serial.println("\nBooting\n");
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  write_output(String("Ready"));
  write_output(String("IP address:"));
  write_output(WiFi.localIP().toString());
  // Region WiFi end

  // Region OTA

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(HOSTNAME);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  // Region OTA end

  // Region Timer
  runner.startNow();  // set point-in-time for scheduling start
  // Region Timer end

  // Region syslog
  syslog.log(LOG_INFO, "End setup function");
  // Region syslog end
}

void loop() {
  // OTA
  ArduinoOTA.handle();
  // Timer
  runner.execute();
}
