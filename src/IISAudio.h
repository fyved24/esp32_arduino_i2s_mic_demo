#ifndef _IISAUDIO_
#define _IISAUDIO_

#include <stdint.h>
#include <Arduino.h>
#include <driver/i2s.h>
#include "config.h"

#endif

const i2s_port_t REC_I2S_PORT = I2S_NUM_0;

extern int32_t *samples_32bit;//读出来的原始32位数据，长度128
extern int16_t *samples_16bit;//转换后的16位数据，长度128
extern uint8_t *samples_8bit ;//转换后的8位数据，长度128

// extern Audio audio; // Audio对象

int32_t *samples_32bit; // 读出来的原始32位数据，长度128
int16_t *samples_16bit; // 转换后的16位数据，长度128
uint8_t *samples_8bit;  // 转换后的8位数据，长度128

// mic读取音频配置
const i2s_config_t rec_i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = I2S_READ_LEN, // 单位是bits_per_sample
};
// mic读取音频引脚配置
const i2s_pin_config_t rec_pin_config = {
    .bck_io_num = INMP441_SCK_PIN, // BCLK
    .ws_io_num = INMP441_WS_PIN,   // LRCL
    .data_out_num = -1,            // not used (only for speakers)
    .data_in_num = INMP441_SD_PIN  // DOUT
};


void I2SInit()
{
  samples_32bit = (int32_t *)malloc(sizeof(int32_t) * AUDIO_FREAME_SIZE);
  samples_16bit = (int16_t *)malloc(sizeof(int16_t) * AUDIO_FREAME_SIZE);
  samples_8bit = (uint8_t *)malloc(sizeof(uint8_t) * AUDIO_FREAME_SIZE);

  esp_err_t err;
  err = i2s_driver_install(REC_I2S_PORT, &rec_i2s_config, 0, NULL);
  if (err != ESP_OK) 
  {
    Serial.printf("Failed installing driver: %d\n", err);
    return;
  }

  err = i2s_set_pin(REC_I2S_PORT, &rec_pin_config);
  if (err != ESP_OK) 
  {
    Serial.printf("Failed installing driver: %d\n", err);
    return;
  }; 
}

int I2Sread(int16_t *samples, int count)
{
  size_t bytes_read = 0;
  if (count > AUDIO_FREAME_SIZE)
  {
    count = AUDIO_FREAME_SIZE; // 最少读取128
  }
  i2s_read(REC_I2S_PORT, (char *)samples_32bit, sizeof(int32_t) * count, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);
  for (int i = 0; i < samples_read; i++)
  {
    int32_t temp = samples_32bit[i] >> 11;
    samples[i] = (temp > INT16_MAX) ? INT16_MAX : (temp < -INT16_MAX) ? -INT16_MAX
                                                                      : (int16_t)temp;
  }
  return samples_read;
}
