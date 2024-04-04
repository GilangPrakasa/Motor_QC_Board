#pragma once
#include <WiFi.h>

// WIFI & MQTT Credentials
#include "credentials.h"
const char wifiSsid[] = WEBSERVER_WIFI_SSID;
const char wifiPass[] = WEBSERVER_WIFI_PASS;

class Wifi_Handler
{   
    public:
        void connect();
        bool reconnect();
        bool isConnected();
        void wifiSetup();
};

extern Wifi_Handler wifiHandler;
extern WiFiClient client;