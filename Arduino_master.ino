//#include <WireRtcLib.h>

#include <DS3231.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include<DHT.h>
#include<Servo.h>
#include "FPS_GT511C3.h"
#include "LiquidCrystal.h"
#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial nodemcu(2,3);
LiquidCrystal lcd(8,9,10,11,12,13);
FPS_GT511C3 fps(7, 6);
DS3231 rtc(A1, A0);
Servo myServo;
int cloth = A2;
int flash = A3;
int state = 0;
int state2 = 0;
int LEDpin = 4;
String data,rtc_time,b;
float temp=0,humi=0;
int pos = 0;
String rem="YouForgotToPut1";
void setup()
{
  rtc.begin();
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  nodemcu.begin(115200);
  fps.Open();
  fps.SetLED(true);
  myServo.attach(4);
  pinMode(cloth,INPUT_PULLUP);
 //rtc.setTime(9,10,00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(23,11, 2018);
}
void suggest()
{
  lcd.clear();
  temp += dht.readTemperature();
  if(temp<20)
{
lcd.setCursor(0,0);
lcd.print("Currently it is");
lcd.setCursor(0,1);
lcd.print(temp);
lcd.print(" C");
delay (5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Advisable to ");
lcd.setCursor(0,1);
lcd.print("wear fur and boots.");
delay (5000);
lcd.clear();
}
else if(temp>=20 && temp<=25)
{
lcd.setCursor(0,0);
lcd.print("Currently it is");
lcd.setCursor(0,1);
lcd.print(temp);
lcd.print(" C");
delay (5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Suggested a warm");
lcd.setCursor(0,1);
lcd.print("jacket and jeans");
delay (5000);
lcd.clear();
}
else if(temp<30)
{
lcd.setCursor(0,0);
lcd.print("Currently it is");
lcd.setCursor(0,1);
lcd.print(temp);
lcd.print(" C");
delay (5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("A shirt with");
lcd.setCursor(0,1);
lcd.print("trousers is best");
delay (5000);
lcd.clear();
}
else
{
lcd.setCursor(0,0);
lcd.print("Currently it is");
lcd.setCursor(0,1);
lcd.print(temp);
lcd.print(" C");
delay (5000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Wear half ");
lcd.setCursor(0,1);
lcd.print("shirt & shorts.");
delay (5000);
lcd.clear();
}
temp=0;
}

void push()
{    
  //rtc_time = rtc.getTimeStr();
 //temp += dht.readTemperature();
  //humi += dht.readHumidity();
  //data="";
//data+="GET /pushingbox?devid=v43B086F4888549C&time=" +rtc_time+ "&temp=" +String(temp)+ "&hum=" +String(humi); //GET request query to pushingbox API
//data+=" HTTP/1.1"; 
//Serial.println(data);
//Serial.println(temp);
//Serial.println(humi);
//temp=0;
//humi=0;
nodemcu.println("l");

//Serial.println("sent signal");
}
void RTC()
{
  lcd.setCursor(0,0);
  lcd.print("Time:" );
  lcd.print(rtc.getTimeStr()); 

  lcd.setCursor(0,1);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());
}
void openn()
{
  for (pos = 0; pos <= 180; pos += 1)
      {
        myServo.write(pos); 
        delay(15);
       
      }
      delay(5000);
      for (pos = 180; pos >= 0; pos -= 1) 
      {
        myServo.write(pos);
        delay(15); 
                     
      }
}
void checkkeys()
{
  state = digitalRead(cloth);
  if(state == LOW)
  {
    lcd.print("Please wait..");
    delay(2000);
    suggest();
    
  }
 /* state2 = digitalRead(flash);
  if(state2 == LOW)
  {
    digitalWrite(LED, HIGH);
    delay(5000);          // wait for 1 second.
    digitalWrite(LED, LOW);
  }*/
}
void loop() 
{
  checkkeys();
  while(Serial.available())
  {
    b="";
    rem=Serial.readString();
         while(Serial.available())
     {
         char t= Serial.read(); 
         b=b+t;
     }
    Serial.end();
    Serial.begin(9600);
  }/*
  if(rem=="unlock");
  {
    openn();
    rem="YouForgotToPut1";
  }*/
  lcd.setCursor(0,0);
  // Identify fingerprint test
  if (fps.IsPressFinger())
  {
    fps.CaptureFinger(false);
    int id = fps.Identify1_N();
    if (id <200)
    {
    lcd.clear();
    lcd.print("   Welcome!   ");
    delay(2000);
    lcd.clear();
    lcd.print("Here is your");
    lcd.setCursor(0,1);
    lcd.print("       reminder");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(rem);
    //lcd.println(id);
    push();
    openn();
      delay(6000);
      lcd.clear();
    }
    else
    {//if unable to recognize
    lcd.clear();
      lcd.println("Finger not found");
      delay(2000);
      lcd.clear();
    }
  }
  else
  {
    RTC();
  }
}
