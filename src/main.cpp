#include <Arduino.h>

// GPIO 2 is the standard internal LED for Freenove WROOM boards
#define BUILTIN_LED 2 
#include "SD_Helper.h" //import the code for the sdcard reading

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.println("--- Freenove Standard LED Test ---");
  
  if (initSDCard()) {
        logToSD("/startup.txt", "System rebooted at " + String(millis()));
    }
}

void loop() {
  Serial.println("LED ON");
  digitalWrite(BUILTIN_LED, HIGH);
  
  static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 10000) {
        int dummySoundLevel = random(0, 100); 
        logToSD("/data.csv", String(millis()) + "," + String(dummySoundLevel));
        lastUpdate = millis();
    }
}