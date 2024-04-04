#include <Arduino.h>
#include "Internet/Wifi_Handler.h"
#include "Internet/Mqtt_Handler.h"
#include "Sensors/Sensors_Handler.h"

// variable millis
long lastMsg = 0;
char msg[50];
int value = 0;
// continous button mode
const int cnts_btn = 18;
int cntsState = 0;
int lastcntsState = 0;
// counter button mode
const int cntr_btn = 19;
int cntrState = 1;
int lastcntrState = 1;
// switching button gsheet 12v to 24v
const int swtch_btn = 4;
int swtchState = 0;
// topic mqtt
String mode_gsheet = "motortestbed/thrower24";
// String mode_gsheet_cnts = "motortestbed/continous24";
// String mode_gsheet_cntr = "motortestbed/counter24";

void setup()
{
  pinMode(cnts_btn, INPUT);
  pinMode(cntr_btn, INPUT);
  pinMode(swtch_btn, INPUT);
  delay(50);
  Serial.begin(115200);
  Serial.println("setup");
  delay(2000);
  wifiHandler.wifiSetup();
  delay(2000);
  mqttHandler.setup_mqtt();
  delay(1000);
  sensorsHandler.begin();
  Serial.println("loop");
}

void loop()
{ 
  if (!wifiHandler.isConnected()) 
  {
    delay(500);
    wifiHandler.reconnect();
  }
  mqttHandler.loop();

  //SWITCHING GSHEET
  swtchState = digitalRead(swtch_btn);
  if(swtchState == LOW)
  {
    int multi1 = digitalRead(5);
    if (multi1 == HIGH)
    {
      // mode_gsheet_cnts.replace("continous24","continousMulti");
      // mode_gsheet_cntr.replace("counter24","counterMulti");
      // mode_gsheet_cnts.replace("continous12","continousMulti");
      // mode_gsheet_cntr.replace("counter12","counterMulti");
    }
    else
    {
      mode_gsheet = "motortestbed/thrower24";
      // mode_gsheet_cnts = "motortestbed/continous24";
      // mode_gsheet_cntr = "motortestbed/counter24";
    }
    
  }
  else
  {
    int multi2 = digitalRead(5);
    if (multi2 == HIGH)
    {
      // mode_gsheet_cnts.replace("continous24","continousMulti");
      // mode_gsheet_cntr.replace("counter24","counterMulti");
      // mode_gsheet_cnts.replace("continous12","continousMulti");
      // mode_gsheet_cntr.replace("counter12","counterMulti");
    }
    else
    {
      mode_gsheet.replace("thrower24","dosing12");
      // mode_gsheet_cnts.replace("24","12");
      // mode_gsheet_cntr.replace("24","12");
    } 
  }

  //MODE CONTINOUS
  sw:
  cntsState = digitalRead(cnts_btn);
  if (cntsState == HIGH)
  {
    long now = millis();
    if (now - lastMsg > 2000)
    { 
      lastMsg = now;
      Serial.print(mode_gsheet);
      // Serial.print(mode_gsheet_cnts);
      SensorsHandler_Value sensorsValue;
      sensorsHandler.getValue(sensorsValue);
      mqttHandler.jsonFormatSensorValue(sensorsValue);
      mqttHandler.mqttPublishSensorValue(mode_gsheet);
      // mqttHandler.mqttPublishSensorValue(mode_gsheet_cnts);
      Serial.println("********************CTS***********************");
      if (cntsState == HIGH)
      {
        goto sw;
      }
    }
  }

  //MODE COUNTER
  cntrState = digitalRead(cntr_btn);
  if (cntrState != lastcntrState) 
  {
    if (cntrState == HIGH) 
    {
      for (int i=0; i<15;i++)
      {
        Serial.print(mode_gsheet);
        // Serial.print(mode_gsheet_cntr);
        SensorsHandler_Value sensorsValue;
        sensorsHandler.getValue(sensorsValue);
        mqttHandler.jsonFormatSensorValue(sensorsValue);
        mqttHandler.mqttPublishSensorValue(mode_gsheet);
        // mqttHandler.mqttPublishSensorValue(mode_gsheet_cntr);
        Serial.println("==================CTR=====================");
        for(uint32_t x = millis();  (millis()-x) < 2000;)
        {

        }
      } 
    }
  }
  lastcntrState = cntrState; 
}
