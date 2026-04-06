#include <Arduino.h>
#include <arduinoFFT.h>
#include <driver/i2s.h>
#include <SPI.h>
#include <SD.h>

//custom helpers
#include "RTC_Helper.h"
#include "Wav_Helper.h"

//pin definitions 
#define SD_SCK  18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS   5
#define I2S_WS  33
#define I2S_SD  32
#define I2S_SCK 14

//audio, calibration settings misc.
const uint16_t samples = 512;
const double samplingFrequency = 16000;
double vReal[samples], vImag[samples];
int16_t* ringBuffer = NULL;
uint32_t ringIndex = 0;
bool bufferFull = false, isRecording = false;
File recordFile;
String currentFileName = "";
unsigned long lastThresholdTime = 0;

ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, samplingFrequency);

void setup() {
    Serial.begin(115200);
    initRTC();
    syncSystemTime(); //fix for the 1990 date issue

    // Initialize SD Card
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS)) Serial.println("SD Card Failed!");

    // Initialize I2S Microphone
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .dma_buf_count = 32,
        .dma_buf_len = 128
    };
    i2s_pin_config_t pin_config = {.bck_io_num = I2S_SCK, .ws_io_num = I2S_WS, .data_in_num = I2S_SD};
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

    // Allocate 1 second of pre-roll memory
    ringBuffer = (int16_t*)malloc(16000 * sizeof(int16_t));
}

void loop() {
    int32_t raw_samples[samples];
    size_t bytes_read;
    i2s_read(I2S_NUM_0, &raw_samples, sizeof(raw_samples), &bytes_read, portMAX_DELAY);

    int16_t samples16[samples];
    for (int i = 0; i < samples; i++) {
        vReal[i] = (double)raw_samples[i];
        vImag[i] = 0;
        
        // Convert to 16-bit and add to ring buffer
        samples16[i] = (int16_t)(raw_samples[i] >> 16);
        ringBuffer[ringIndex++] = samples16[i];
        if (ringIndex >= 16000) { ringIndex = 0; bufferFull = true; }
    }

    // FFT Peak Detection (Volume)
    double currentMax = 0;
    for (int i = 4; i < samples/2; i++) if (vReal[i] > currentMax) currentMax = vReal[i];
    double db = (currentMax > 0) ? (20.0 * log10(currentMax)) - 120.0 : 30.0;

    // Threshold Check (45dB)
    if (db > 45.0) {
        lastThresholdTime = millis();
        if (!isRecording) {
            RtcDateTime now = Rtc.GetDateTime();
            char fn[32];
            snprintf(fn, sizeof(fn), "/%04u%02u%02u_%02u%02u%02u.wav", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
            currentFileName = String(fn);
            recordFile = SD.open(currentFileName, FILE_WRITE);
            if (recordFile) {
                writeWavHeader(recordFile, 16000);
                if (bufferFull) {
                    recordFile.write((const byte*)&ringBuffer[ringIndex], (16000 - ringIndex) * 2);
                    recordFile.write((const byte*)&ringBuffer[0], ringIndex * 2);
                }
                isRecording = true;
                Serial.println("Threshold hit! Recording: " + currentFileName);
            }
        }
    }

    // Recording Logic
    if (isRecording) {
        recordFile.write((const byte*)samples16, sizeof(samples16));
        if (millis() - lastThresholdTime >= 2000) { // 2 second post-roll
            recordFile.close();
            finalizeWavHeader(currentFileName);
            isRecording = false;
            Serial.println("Recording Saved.");
        }
    }
}