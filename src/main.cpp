
#include <driver/i2s.h> //for the ics43434 mic

#include <Arduino.h>

void setup() {
  // Use a lower baud rate just in case the data pins are struggling
  Serial.begin(9600); 
  delay(2000); 
  Serial.println("\n\n---STARTING---");
}

void loop() {
  // We use a long delay to keep power consumption as low as possible
  Serial.print("Uptime (ms): ");
  Serial.println(millis());
  delay(2000); 
}