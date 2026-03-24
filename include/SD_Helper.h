#ifndef SD_HELPER_H
#define SD_HELPER_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Define the Chip Select pin based on your wiring
#define SD_CS_PIN 5

// Function to initialize the SD card
bool initSDCard() {
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Card Mount Failed!");
        return false;
    }
    Serial.println("SD Card Initialized.");
    return true;
}

// Function to log data (Append mode)
void logToSD(const char* path, String message) {
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.println(message)) {
        Serial.println("Log Saved: " + message);
    } else {
        Serial.println("Log Write Failed");
    }
    file.close();
}

#endif