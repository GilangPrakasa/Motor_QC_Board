#include "Sensors_Handler.h"

Sensors_Handler sensorsHandler;

// struct untuk data parser serial monitor
int8_t D_AMB1, D_OBJ1, D_VOL1, D_CUR1, D_RPM1, D_AMB2, D_OBJ2, D_VOL2, D_CUR2, D_RPM2;
char c; 
String DataIn;

// hardwareserial pin 
#define RXD2 16
#define TXD2 17

void Sensors_Handler::begin()
{
   Serial.begin(115200);
   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
   while(!Serial2);
   Serial.println("Ready");
}

void Sensors_Handler::getValue(SensorsHandler_Value& sensorsValue)
{
   while(Serial2.available()>0) 
   {
      c= Serial2.read();
      if(c== '\n') 
      {
         break;
      } 
      else 
      {
         DataIn+=c;
      }
   }
   if(c== '\n')
   {
      D_AMB1 = DataIn.indexOf("A");
      D_OBJ1 = DataIn.indexOf("B");
      D_VOL1 = DataIn.indexOf("C");
      D_CUR1 = DataIn.indexOf("D");
      D_RPM1 = DataIn.indexOf("E");
      D_AMB2 = DataIn.indexOf("F");
      D_OBJ2 = DataIn.indexOf("G");
      D_VOL2 = DataIn.indexOf("H");
      D_CUR2 = DataIn.indexOf("I");
      D_RPM2 = DataIn.indexOf("J");
      sensorsValue.AMB1 = DataIn.substring (0, D_AMB1);
      sensorsValue.OBJ1 = DataIn.substring (D_AMB1+1, D_OBJ1);
      sensorsValue.VOL1 = DataIn.substring (D_OBJ1+1, D_VOL1);
      sensorsValue.CUR1 = DataIn.substring (D_VOL1+1, D_CUR1);
      sensorsValue.RPM1 = DataIn.substring (D_CUR1+1, D_RPM1);
      sensorsValue.AMB2 = DataIn.substring (D_RPM1+1, D_AMB2);
      sensorsValue.OBJ2 = DataIn.substring (D_AMB2+1, D_OBJ2);
      sensorsValue.VOL2 = DataIn.substring (D_OBJ2+1, D_VOL2);
      sensorsValue.CUR2 = DataIn.substring (D_VOL2+1, D_CUR2);
      sensorsValue.RPM2 = DataIn.substring (D_CUR2+1, D_RPM2);

      c=0;
      DataIn="";  
   }
   Serial2.end();
   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
   
}