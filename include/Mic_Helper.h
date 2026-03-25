#ifndef MIC_HELPER_H
#define MIC_HELPER_H

#include <driver/i2s.h>

// I2S Pin Configuration
#define I2S_WS 33
#define I2S_SD 32
#define I2S_SCK 14
#define I2S_PORT I2S_NUM_0

void initMicrophone() {
    const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1, // Not used for recording
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
}

int32_t readMicSample() {
    int32_t raw_sample = 0;
    size_t bytes_read = 0;
    i2s_read(I2S_PORT, &raw_sample, sizeof(raw_sample), &bytes_read, portMAX_DELAY);
    
    // The ICS-43434 is 24-bit data inside a 32-bit word, 
    // we shift it to make the numbers manageable.
    return raw_sample >> 14; 
}
#endif