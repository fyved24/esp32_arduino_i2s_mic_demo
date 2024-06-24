#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>
#include "HTTPUpdate.h"
// MQTT配置
String mqttServer = "192.168.31.202";
int mqttPort = 1883;
String recvCommandTopic = "command/"; // command/sn
String audioTopic = "data/audio"; 
String ssid = "link";
String psk = "25802580";

#define AUDIO_FREAME_SIZE 128
// #define SAMPLE_RATE 16000
#define SAMPLE_RATE 44100
#define I2S_READ_LEN (1<<7)

// 麦克风器配置
#define INMP441_WS_PIN  42 // 串行数据字选择
#define INMP441_SCK_PIN 14 //  时钟
#define INMP441_SD_PIN 38  // 数据 
#endif