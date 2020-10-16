#include <Arduino.h>

// Region -- NTC Thermistor
// PIN OF SIGNAL
#define THERMISTORPIN A0    
// resistance of termistor at 25 degrees C
#define THERMISTORNOMINAL 10000      
#define TEMPERATURENOMINAL 25   
// Accuracy - Higher number is bigger
#define NUMSAMPLES 100
// Beta coefficient from datasheet
#define BCOEFFICIENT 3950
// the value of the R1 resistor
#define SERIESRESISTOR 10000    
//prepare pole 
uint16_t samples[NUMSAMPLES];
// Region end -- NTC Thermistor


// Region -- Flow counter
#define FLOWCOUNTPIN D5 // Must allow interrupt
#define FLOWDEBOUNCEDELAY 20
// Flow counter
volatile int Flowstate;
volatile int FlowCounterValue = 0;
// Holds the last time debounce was evaluated (in millis).
volatile long FlowLastDebounceTime = 0;
// Region end -- Flow counter

// Region -- Rain counter
#define RAINCOUNTPIN D6 // Must allow interrupt
#define RAINDEBOUNCEDELAY 1000
// Flow counter
volatile int RainState;
volatile int RainCounterValue = 0;
// Holds the last time debounce was evaluated (in millis).
volatile long RainLastDebounceTime = 0;
// Region end -- Rain counter

float get_temperature() {
  uint8_t i;
  float average;
 
 // saving values from input to pole
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
 
   // average value of input
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  //resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;

 //resistence to temperature
  float temperature;
  temperature = average / THERMISTORNOMINAL; 
  temperature = log(temperature);   
  temperature /= BCOEFFICIENT;                  
  temperature += 1.0 / (TEMPERATURENOMINAL + 273.15); 
  temperature = 1.0 / temperature;                 
  temperature -= 273.15;                         // convert to C
 
  return temperature;
}

// Gets called by the interrupt.
ICACHE_RAM_ATTR void onFlowCounterChange() {
  // Get the pin reading.
  int reading = digitalRead(FLOWCOUNTPIN);

  // Ignore dupe readings.
  if(reading == Flowstate) return;
  
  // Check to see if the change is within a debounce delay threshold.
  if((millis() - FlowLastDebounceTime) <= FLOWDEBOUNCEDELAY) {
    return;
  }

  // This update to the last debounce check is necessary regardless of debounce Flowstate.
  FlowLastDebounceTime = millis();

  // All is good, persist the reading as the Flowstate.
  Flowstate = reading;
  FlowCounterValue++;
}

// Gets called by the interrupt.
ICACHE_RAM_ATTR void onRainCounterChange() {
  // Get the pin reading.
  int reading = digitalRead(RAINCOUNTPIN);

  // Ignore dupe readings.
  if(reading == RainState) return;
  
  // Check to see if the change is within a debounce delay threshold.
  if((millis() - RainLastDebounceTime) <= RAINDEBOUNCEDELAY) {
    return;
  }

  // This update to the last debounce check is necessary regardless of debounce RainState.
  RainLastDebounceTime = millis();

  // All is good, persist the reading as the RainState.
  RainState = reading;
  RainCounterValue++;
}

int get_flowCounter(){
  return (float)FlowCounterValue;
}

float get_rainCounter(){
  // float rain_mm = RainCounterValue * (float)0.3537;

  return (float)RainCounterValue * (float)0.3537;
}

void setupSensors(){
  pinMode(THERMISTORPIN, INPUT);

  pinMode(FLOWCOUNTPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOWCOUNTPIN), onFlowCounterChange, CHANGE);
  
  pinMode(RAINCOUNTPIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RAINCOUNTPIN), onRainCounterChange, CHANGE);
}
