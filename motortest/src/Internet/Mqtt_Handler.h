#pragma once
// MQTT, WiFi Handler, and timestamp
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "Sensors/Sensors_Handler.h"
#include "credentials.h"

// Add your MQTT Broker IP address, example:
const char mqttServer[] = MQTT_SERVER;
const char mqttUser[]   = MQTT_USER;
const int  mqttPort  = MQTT_PORT;
const char mqttPass[]   = MQTT_PASS;
const char mqttClientName[] = MQTT_CLIENT;

// extern bool sensor_switch;
extern PubSubClient mqttClient;

class Mqtt_Handler
{
    public:
        void setup_mqtt();
        void reconnect_loop();
        void loop();
        bool isWifiConnected();
        bool isMqttConnected();
        void jsonFormatSensorValue(SensorsHandler_Value& value);
        void mqttPublishSensorValue(String _topic);
    private:
        //char epochTime;
        String dataFormatjson;
};

void callback(char *topic, byte *message, unsigned int length);
unsigned long getTime();

extern Mqtt_Handler mqttHandler;

