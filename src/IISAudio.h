#ifndef _I2SMIC_
#define _I2SMIC_

#include <stdint.h>
#include <Arduino.h>
#include <driver/i2s.h>
#include "config.h"

#define _AUDIO_FRAME_SIZE 128
#define _SAMPLE_RATE 16000

class I2SMic {
public:
    I2SMic(int sample_rate = _SAMPLE_RATE, int audio_frame_size=_AUDIO_FRAME_SIZE, i2s_port_t i2s_port = I2S_NUM_0)
        : sample_rate(sample_rate), audio_frame_size(audio_frame_size), i2s_port(i2s_port), samples_32bit(nullptr), samples_16bit(nullptr) {
        samples_32bit = (int32_t *)malloc(sizeof(int32_t) * audio_frame_size);
    }

    ~I2SMic() {
        if (samples_32bit) free(samples_32bit);
        i2s_driver_uninstall(i2s_port);
    }

    void init() {
        i2s_config_t i2s_config = {
            .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
            .sample_rate = sample_rate,
            .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
            .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
            .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
            .dma_buf_count = 8,
            .dma_buf_len = 1024,
        };

        esp_err_t err;
        err = i2s_driver_install(i2s_port, &i2s_config, 0, NULL);
        if (err != ESP_OK) {
            Serial.printf("Failed installing driver: %d\n", err);
            return;
        }

        err = i2s_set_pin(i2s_port, &pin_config);
        if (err != ESP_OK) {
            Serial.printf("Failed setting pin: %d\n", err);
            return;
        }
    }

    void setPinout(int bck, int ws, int data_in) {
        pin_config = {
            .bck_io_num = bck,
            .ws_io_num = ws,
            .data_out_num = -1,
            .data_in_num = data_in
        };
    }

    int read(int16_t *samples, int count) {
        size_t bytes_read = 0;
        if (count > audio_frame_size) {
            count = audio_frame_size;
        }
        i2s_read(i2s_port, (char *)samples_32bit, sizeof(int32_t) * count, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        for (int i = 0; i < samples_read; i++) {
            int32_t temp = samples_32bit[i] >> 11;
            samples[i] = (temp > INT16_MAX) ? INT16_MAX : (temp < -INT16_MAX) ? -INT16_MAX : (int16_t)temp;
        }
        return samples_read;
    }

private:
    i2s_port_t i2s_port;
    int sample_rate;
    int audio_frame_size;
    int32_t *samples_32bit;
    int16_t *samples_16bit;
    i2s_pin_config_t pin_config;
};

#endif
