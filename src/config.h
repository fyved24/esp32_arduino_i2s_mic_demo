#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>
#include "HTTPUpdate.h"
// MQTT配置
String mqttServer = "192.168.99.95";
int mqttPort = 1883;
String recvCommandTopic = "command/"; // command/sn
String audioTopic = "data/audio"; 
String ssid = "k70";
String psk = "25802580";


// 麦克风器配置
#define INMP441_WS_PIN  42 // 串行数据字选择
#define INMP441_SCK_PIN 41 //  时钟
#define INMP441_SD_PIN 40  // 数据 
#endif