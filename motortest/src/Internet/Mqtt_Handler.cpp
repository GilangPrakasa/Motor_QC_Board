#include "Mqtt_Handler.h"

// Indicator LED Pin
const int led1 = 32;
const int led2 = 33;

Mqtt_Handler mqttHandler;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// setting time & server
const char* ntpServer = "0.id.pool.ntp.org";
const long  utc       = 6 * 3600;
const int   sec       = 3600;

void Mqtt_Handler::setup_mqtt()
{   
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(0, INPUT);
    pinMode(4, INPUT);
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.setCallback(callback);
    // init configTime
    configTime(utc, sec, ntpServer);
}

void Mqtt_Handler::reconnect_loop()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ESP32Client"))
    {
      Serial.println("connected");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      // Subscribe
      mqttClient.subscribe("ketimpangan/switch");
      mqttClient.subscribe("ketimpangan/LED");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 1 seconds");
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}

void Mqtt_Handler::loop() 
{
    if (!mqttClient.connected())
    {
        reconnect_loop();
    }
    mqttClient.loop();
}

void Mqtt_Handler::jsonFormatSensorValue(SensorsHandler_Value& value)
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    
    return;
  }
  getLocalTime(&timeinfo);

  char epochTime[33];
  strftime(epochTime,33, "%A %B %d %Y %H:%M:%S", &timeinfo);
  Serial.print(" Time: ");
  Serial.println(&timeinfo, "%A %B %d %Y %H:%M:%S");

  String clientId = WiFi.macAddress();
  clientId.replace(':',' ');
  clientId.toUpperCase();

  // SWITCHING 
  int multiple = digitalRead(5);
  if(multiple == HIGH)
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    dataFormatjson = "{\"AMB1\":" + value.AMB1 + 
                    ",\"OBJ1\":" + value.OBJ1 +
                    ",\"VOL1\":" + value.VOL1 + 
                    ",\"CUR1\":" + value.CUR1 + 
                    ",\"RPM1\":" + value.RPM1 +
                    ",\"AMB2\":" + value.AMB2 +
                    ",\"OBJ2\":" + value.OBJ2 +
                    ",\"VOL2\":" + value.VOL2 + 
                    ",\"CUR2\":" + value.CUR2 + 
                    ",\"RPM2\":" + value.RPM2 +
                    ",\"time\":\"" + String(epochTime) + "\"" + 
                    ",\"clientId\":\"" + String(clientId) + "\"}";
  }
  else
  {
    int changeReader = digitalRead(4);
    if(changeReader == LOW)
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      dataFormatjson = "{\"AMB1\":" + value.AMB1 + 
                      ",\"OBJ1\":" + value.OBJ1 +
                      ",\"VOL1\":" + value.VOL1 + 
                      ",\"CUR1\":" + value.CUR1 + 
                      ",\"RPM1\":" + value.RPM1 +
                      ",\"time\":\"" + String(epochTime) + "\"" + 
                      ",\"clientId\":\"" + String(clientId) + "\"}";
    }
    else
    {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      dataFormatjson = "{\"AMB2\":" + value.AMB2 + 
                      ",\"OBJ2\":" + value.OBJ2 +
                      ",\"VOL2\":" + value.VOL2 + 
                      ",\"CUR2\":" + value.CUR2 + 
                      ",\"RPM2\":" + value.RPM2 +
                      ",\"time\":\"" + String(epochTime) + "\"" + 
                      ",\"clientId\":\"" + String(clientId) + "\"}";
    }
  }

  
  Serial.print("Packet : ");
  Serial.println(dataFormatjson);
}

void Mqtt_Handler::mqttPublishSensorValue(String _topic)
{
    mqttClient.publish(_topic.c_str(), dataFormatjson.c_str(), true);
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  // if (String(topic) == "ketimpangan/LED")
  // {
  //   Serial.print("Changing output to ");
  //   if (messageTemp == "on")
  //   {
  //     Serial.println("on");
  //   }
  //   else if (messageTemp == "off")
  //   {
  //     Serial.println("off");
  //   }
  // }
  // if (String(topic) == "ketimpangan/switch")
  // {
  //   Serial.print("Changing output to ");
  //   if (messageTemp == "on")
  //   {
  //     Serial.println("on");
  //   }
  //   else if (messageTemp == "off")
  //   {
  //     Serial.println("off");
  //   }
  // }
}

