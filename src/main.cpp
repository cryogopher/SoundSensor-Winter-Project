#include <Arduino.h>

//GPIO 2 internal LED for Freenove WROOM boards
#define BUILTIN_LED 2 
#include "SD_Helper.h" //import the code for the sdcard reading
#include "RTC_Helper.h"
#include "Mic_Helper.h"

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.println("--- Freenove Standard LED Test ---");
  Serial.println("LED ON");

  initSDCard();
  initRTC();
  initMicrophone();

  /*
  digitalWrite(BUILTIN_LED, HIGH);
  if (initSDCard()) {
        logToSD("/startup.txt", "System rebooted at " + String(millis()));
    }
  */

}

void loop() {

  // Read a single audio sample
    int32_t sample = readMicSample();

    // 1. VISUAL OUTPUT: Print the number for the Serial Plotter
    Serial.println(sample);

    // 2. LOGGING: Every 1 second, log a summary to the SD card
    static unsigned long lastLog = 0;
    if (millis() - lastLog > 1000) {
        String logEntry = getTimestamp() + " | Peak: " + String(sample);
        logToSD("/audio_log.txt", logEntry);
        lastLog = millis();
    }
  String currentTime = getTimestamp(); 

}