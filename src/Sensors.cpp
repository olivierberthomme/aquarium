#include <Arduino.h>

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
