// gilangprakasa99@gmail.com
// Sensor Handle Motor Test 1.0

#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>
#define IR1 0x5A
#define IR2 0x5B

Adafruit_MLX90614 mlx;
SoftwareSerial Arduino_SoftSerial (12,11); // RX,TX
LiquidCrystal_I2C lcd(0x27, 20, 4);

float dt1, dt2, dt3, dt4, dt5, dt6, dt7, dt8, dt9, dt10;

long lastMsg = 0;

float rpm1 = 0;
unsigned long millisBefore1;
volatile int holes1;

float rpm2 = 0;
unsigned long millisBefore2;
volatile int holes2;

long int ADC_Cur1 = 0;
float Vol_Cur1 = 0;
float Current1 = 0;

long int ADC_Cur2 = 0;
float Vol_Cur2 = 0;
float Current2 = 0;

long int ADC_Vol1 = 0;
float Vol_Vol1 = 0;
float Voltage1 = 0;

long int ADC_Vol2 = 0;
float Vol_Vol2 = 0;
float Voltage2 = 0;

void setup() 
{
  Serial.begin(115200);
  Arduino_SoftSerial.begin(9600);

  analogReference(EXTERNAL);

  mlx.begin();

  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(3), count1, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), count2, FALLING);

  lcd.begin();
  lcd.backlight();
  delay(1000);
  lcd.clear();

  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void loop() 
{
  for (int i = 0; i < 250; i++)
  {
    ADC_Cur1 += analogRead(A2);
    ADC_Cur2 += analogRead(A3);
    for(uint32_t x = millis();  (millis()-x) < 2;)
    {

    }
  }
  ADC_Cur1 = ADC_Cur1 / 250;
  Vol_Cur1 = ADC_Cur1 * 4.98 / 1023.0;
  Current1 = (Vol_Cur1 - 2.65) / 0.122; 

  ADC_Cur2 = ADC_Cur2 / 250;
  Vol_Cur2 = ADC_Cur2 * 4.98 / 1023.0;  
  Current2 = (Vol_Cur2 - 2.15) / 0.217; 

  if(Current1 >= 14.00 || Current2 >= 14.00)
  {
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
  }

  for (int j = 0; j < 250; j++)
  {
    ADC_Vol1 += analogRead(A6);
    ADC_Vol2 += analogRead(A7);
    for(uint32_t z = millis();  (millis()-z) < 2;)
    {

    }
  }
  ADC_Vol1 = ADC_Vol1 / 250;
  Vol_Vol1 = ADC_Vol1 * 4.98 / 1023.0;
  Voltage1 = Vol_Vol1 / (209.33 / (952.0 + 209.33));

  ADC_Vol2 = ADC_Vol2 / 250;
  Vol_Vol2 = ADC_Vol2 * 4.98 / 1023.0;
  Voltage2 = Vol_Vol2 / (325.11 / (465.0 + 325.11)); //-R2 jika Vreff > Voltage2 || +R2 jika Vreff < Voltage2

  long now = millis();
  if (now - lastMsg > 500)
  {
  lastMsg = now;    

  mlx.AddrSet(IR1);    
  float ambient_temp1 = mlx.readAmbientTempC();
  float object_temp1 = mlx.readObjectTempC();
  mlx.temp1 = mlx.readObjectTempC();

  mlx.AddrSet(IR2);    
  float ambient_temp2 = mlx.readAmbientTempC();
  float object_temp2 = mlx.readObjectTempC();
  mlx.temp2 = mlx.readObjectTempC();

  rpm_read1();
  rpm_read2();

  dt1 = ambient_temp1;
  dt2 = object_temp1;
  dt3 = Voltage1;
  dt4 = Current1; 
  dt5 = rpm1;
  dt6 = ambient_temp2; 
  dt7 = object_temp2; 
  dt8 = Voltage2;
  dt9 = Current2;
  dt10 = rpm2;

  Arduino_SoftSerial.begin(9600);

  Arduino_SoftSerial.print(dt1, 2);      Arduino_SoftSerial.print("A");
  Arduino_SoftSerial.print(dt2, 2);      Arduino_SoftSerial.print("B");
  Arduino_SoftSerial.print(dt3, 2);      Arduino_SoftSerial.print("C"); 
  Arduino_SoftSerial.print(dt4, 2);      Arduino_SoftSerial.print("D"); 
  Arduino_SoftSerial.print(dt5, 0);      Arduino_SoftSerial.print("E"); 
  Arduino_SoftSerial.print(dt6, 2);      Arduino_SoftSerial.print("F");
  Arduino_SoftSerial.print(dt7, 2);      Arduino_SoftSerial.print("G");
  Arduino_SoftSerial.print(dt8, 2);      Arduino_SoftSerial.print("H"); 
  Arduino_SoftSerial.print(dt9, 2);      Arduino_SoftSerial.print("I"); 
  Arduino_SoftSerial.print(dt10, 0);      Arduino_SoftSerial.print("J");
  Arduino_SoftSerial.print("\n");
  for(uint32_t y = millis();  (millis()-y) < 500;)
  {

  }
  Arduino_SoftSerial.end();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TMP|");
  lcd.setCursor(5, 0);
  lcd.print(object_temp1,2);
  lcd.setCursor(12, 0);
  lcd.print(object_temp2,2);
  lcd.setCursor(0, 1);
  lcd.print("VOL|");
  lcd.setCursor(5, 1);
  lcd.print(Voltage1,2);  
  lcd.setCursor(12, 1);
  lcd.print(Voltage2,2);
  lcd.setCursor(0, 2);
  lcd.print("CUR|");
  lcd.setCursor(5, 2);
  lcd.print(Current1,2);
  lcd.setCursor(12, 2);
  lcd.print(Current2,2);
  lcd.setCursor(0, 3);
  lcd.print("RPM|");
  lcd.setCursor(5, 3);
  lcd.print(rpm1,0);
  lcd.setCursor(12, 3);
  lcd.print(rpm2,0);
  }
}

void rpm_read1()
{
  if (millis() - millisBefore1 > 1000) {
    rpm1 = (holes1 / 4.0)*60;
    holes1 = 0;
    millisBefore1 = millis();
  }
  delay(100);
}
void count1() 
{
  holes1++;
}

void rpm_read2()
{
  if (millis() - millisBefore2 > 1000) {
    rpm2 = (holes2 / 4.0)*60;
    holes2 = 0;
    millisBefore2 = millis();
  }
  delay(100);
}
void count2() 
{
  holes2++;
}
