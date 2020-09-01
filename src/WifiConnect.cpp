#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <secrets.h>
#include <TaskScheduler.h>
#include <Syslog.h>

#define HOSTNAME "RobotPotager"
// Scheduler variables
#define _TASK_SLEEP_ON_IDLE_RUN

// Wifi variables
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;

// Syslog server connection info
#define SYSLOG_SERVER "192.168.1.100"
#define SYSLOG_PORT 10500

// Prototype
void readWiFiSignal();

// Timer/Scheduler
Scheduler runner;
// List tasks
Task t2(2*60*1000, TASK_FOREVER, &readWiFiSignal, &runner, true);  //adding task to the chain on creation

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, HOSTNAME, LOG_KERN);

void write_output(String message){
  Serial.println(message);
  syslog.log(LOG_INFO, message);
}

void readWiFiSignal() {
    int signal = WiFi.RSSI();
    char message[64];
    sprintf(message, "Wifi signal: %d dB", signal);
    write_output(message);
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
