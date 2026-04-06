#ifndef WAV_HELPER_H
#define WAV_HELPER_H

#include <SD.h>

void writeWavHeader(File file, uint32_t sampleRate) {
    byte header[44];
    uint16_t channels = 1;
    uint16_t bitsPerSample = 16;
    uint32_t byteRate = sampleRate * channels * bitsPerSample / 8;

    memcpy(&header[0], "RIFF", 4);
    uint32_t fileSize = 0; 
    memcpy(&header[4], &fileSize, 4);
    memcpy(&header[8], "WAVE", 4);
    memcpy(&header[12], "fmt ", 4);
    uint32_t fmtLen = 16;
    memcpy(&header[16], &fmtLen, 4);
    uint16_t fmtType = 1; 
    memcpy(&header[20], &fmtType, 2);
    memcpy(&header[22], &channels, 2);
    memcpy(&header[24], &sampleRate, 4);
    memcpy(&header[28], &byteRate, 4);
    uint16_t blockAlign = channels * bitsPerSample / 8;
    memcpy(&header[32], &blockAlign, 2);
    memcpy(&header[34], &bitsPerSample, 2);
    memcpy(&header[36], "data", 4);
    uint32_t dataLen = 0; 
    memcpy(&header[40], &dataLen, 4);
    file.write(header, 44);
}

void finalizeWavHeader(String filename) {
    File file = SD.open(filename, "r+");
    if (file) {
        uint32_t fileSize = file.size() - 8;
        uint32_t dataSize = file.size() - 44;
        file.seek(4);
        file.write((byte*)&fileSize, 4);
        file.seek(40);
        file.write((byte*)&dataSize, 4);
        file.close();
    }
}
#endif