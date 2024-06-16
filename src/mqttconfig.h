#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H
// #include <PubSubClient.h>
#include <ArduinoMqttClient.h>
#include "netconfig.h"

MqttClient mqttClient(wifiClient);

//连接mqtt，并订阅subTopic
void mqttInit()
{

    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(mqttServer);
    // int i=20;
    while (!mqttClient.connect(mqttServer.c_str(), mqttPort))
    {
        // if(i==0){
        //     ESP.restart();
        // }
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        delay(1000);
        // i--;
    }
    Serial.println("You're connected to the MQTT broker!");
    // subscribe to a topic
    Serial.print("Subscribing to topic: ");
    Serial.println(recvCommandTopic);
    // pblish to a topic
    Serial.print("Publishing audio data to topic: ");
    mqttClient.subscribe(recvCommandTopic, 2);
    Serial.println(audioTopic);


}

//重连后会重新订阅主题
void reconnectMqtt(void (*callback)(int))
{
    if (!mqttClient.connected())
    {
        Serial.println("MQTT Not Connected");
        int i=10;
        while (!mqttClient.connect(mqttServer.c_str(), mqttPort))
        {
            // 以太网dhcp连接后，路由器更换网段，esp32还是会使用原来已获取的地址，这里ip指定为0.0.0.0相当于重新获取动态ip
            Serial.print("MQTT connection failed! Error code = ");
            Serial.println(mqttClient.connectError());
            delay(1000);
            i--;
        }
        Serial.println("You're connected to the MQTT broker!");
        // subscribe to a topic
        Serial.print("Subscribing to topic: ");
        Serial.println(recvCommandTopic);
        // pblish to a topic
        Serial.print("Publishing audio data to topic: ");
        mqttClient.subscribe(recvCommandTopic, 2);
        Serial.println(audioTopic);
        delay(1000);
    }
}


void sendAudioData(const uint8_t *data, uint16_t len)
{
    mqttClient.beginMessage(audioTopic, false, 0, false);
    mqttClient.write(data, len);
    mqttClient.endMessage();
}
#endif
