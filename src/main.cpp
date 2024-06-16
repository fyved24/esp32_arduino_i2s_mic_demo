
#include "HardwareSerial.h"
#include <Arduino.h>
#include "config.h"
#include "netconfig.h"
#include "mqttconfig.h"
#include "IISAudio.h"


#define AUDIO_FRAME_SIZE 128
#define SAMPLE_RATE 16000

I2SMic mic(SAMPLE_RATE, AUDIO_FRAME_SIZE);

void setup()
{
    // pinMode(BLUE_LED_PIN, OUTPUT);

    Serial.begin(115200);
    setupWifi();
    // I2S初始化
    mic.setPinout(INMP441_SCK_PIN, INMP441_WS_PIN, INMP441_SD_PIN);
    mic.init();
    mqttInit();
}
// 当mqtt收到消息以及重连时触发，对收到的消息进行解析，通常为远程指令
void onMqttMessage(int messageSize)
{
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");
}

int16_t samples_16bit[AUDIO_FRAME_SIZE];

void loop()
{
    // 检查MQTT客户端是否有待处理的消息或事件
    mqttClient.poll();
    unsigned long now = millis();
    // 重连成功时触发回调函数onMqttMessage，
    reconnectMqtt(onMqttMessage);
    int samples_read = mic.read(samples_16bit, AUDIO_FRAME_SIZE); // 读取音频数据
    // 由于发送只能发 uint8_t，所以16bit数组，长度要*2
    sendAudioData((uint8_t *)samples_16bit, samples_read * 2);    // 发射音频数据
}
