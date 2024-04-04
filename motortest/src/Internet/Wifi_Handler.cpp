#include "Wifi_Handler.h"

Wifi_Handler wifiHandler;
WiFiClient client;

void Wifi_Handler::connect() {
    wifiSetup();
}

bool Wifi_Handler::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

bool Wifi_Handler::reconnect() {
    WiFi.disconnect();
    WiFi.reconnect();
    return isConnected();
}

void Wifi_Handler::wifiSetup(){
    // Connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifiSsid);
    WiFi.begin(wifiSsid, wifiPass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");

        long startTime = millis();
        if (millis() - startTime > 5000)
            ESP.restart();  // restart if taking too long
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
