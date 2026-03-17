#include <Arduino.h>
#include <driver/i2s.h> //for the ics43434 mic

#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32 Initialized in PlatformIO!");
  Serial.println("\n--- NEW UPLOAD V1.0 EXECUTING ---");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);

  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
