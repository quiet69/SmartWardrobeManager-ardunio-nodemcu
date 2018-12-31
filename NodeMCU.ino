#include "ESP8266WiFi.h"
#include<DHT.h>
#include <Wire.h>
#define DHTPIN D5
#define DHTTYPE DHT11
#define LED D3

DHT dht(DHTPIN, DHTTYPE);

String data;
float temp=0,humi=0;
const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid1 = "Your_pushdeviceID";//device ID from Pushingbox 
const String devid2 = "Your_emaildeviceID";  
const char* MY_SSID = "Your_ssid";
const char* MY_PWD =  "Your_password";
boolean status = false;
int z = 1;
String b,yo;
int buzz = D8;
int ir = D7;
int isObstacle = LOW;
int amz = D1;
int state = 0;
void setup() 
{
  pinMode(buzz,OUTPUT);
  pinMode(ir,INPUT);
  pinMode(LED, OUTPUT);
  pinMode(amz,INPUT_PULLUP);
  Serial.begin(115200);
  //Serial.print("Connecting to "+*MY_SSID);
 WiFi.begin(MY_SSID, MY_PWD); // put your setup code here, to run once:
 Serial.println("going into wl connect");
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  while (WiFi.status() != WL_CONNECTED) //not connected,..waiting to connect
    {
      delay(1000);
      //Serial.print(".");
    }
}
void push()
{
 // rtc_time = rtc.getTimeStr();
 temp += dht.readTemperature();
  humi += dht.readHumidity();
  data="";
data+="GET /pushingbox?devid="+devid1+"&temp=" +String(temp)+ "&hum=" +String(humi); //GET request query to pushingbox API
data+=" HTTP/1.1"; 
delay(5000); //10 seconds, (sampling rate vs. service call quota)
WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
             // Serial.println("connected");
              client.println(data);
              client.println("Host: api.pushingbox.com");
              client.println("Connection: close");
              client.println();
              status = true;
      }
     else
     {
      //Serial.println("connection failed");
      }
      while(status)
      {
            if (client.available())   
                    {
                        char c = client.read(); //save http header to c
                        //Serial.print(c); //print http header to serial monitor
                    }
             if (!client.connected()) 
                    {
                        //Serial.println();
                        //Serial.println("disconnecting.");
                        //Serial.print("Data sent :");
                        client.stop();
                        status = false;
                        data = ""; //data reset
                    }
      }
temp=0;
humi=0;
delay(10000);
}
void checkkeys()
{
  state = digitalRead(amz);
  if(state == LOW)
  {
    //lcd.print("Please wait..");
    //delay(2000);
    amazon();
  }
}
void amazon()
{
  temp += dht.readTemperature();
  if(temp<=20)
{ 
  yo="www.myntra.com/winter-wear";
}
else if(temp>=20 && temp<=25)
{
  yo="https://www.myntra.com/jeans,www.myntra.com/t-shirts";
}
else if(temp<30)
{
  yo="www.myntra.com/formals";
}
else
{
  yo="www.myntra.com/shorts,www.myntra.com/tank-tops";
}
temp=0;
data="";
data+="GET /pushingbox?devid="+devid2+"&data=" + String(yo); //GET request query to pushingbox API
data+=" HTTP/1.1";
 
  WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
             // Serial.println("connected");
              client.println(data);
              client.println("Host: api.pushingbox.com");
              client.println("Connection: close");
              client.println();
              status = true;
      }
     else
     {
      //Serial.println("connection failed");
      }
  while(status)
      {
            if (client.available())   
                    {
                        char c = client.read(); //save http header to c
                        //Serial.print(c); //print http header to serial monitor
                    }
             if (!client.connected()) 
                    {
                        //Serial.println();
                        //Serial.println("disconnecting.");
                        //Serial.print("Data sent :");
                        client.stop();
                        status = false;
                        data = ""; //data reset
                    }
      }
}
void alert()
{
  yo="AlertSecurityBreach!!!";

data="";
data+="GET /pushingbox?devid="+devid2+"&data=" + String(yo); //GET request query to pushingbox API
data+=" HTTP/1.1";
delay(5000);
  WiFiClient client;  //Instantiate WiFi object

    //Start or API service using our WiFi Client through PushingBox
    if (client.connect(WEBSITE, 80))
      { 
             Serial.println("connected");
              client.println(data);
              client.println("Host: api.pushingbox.com");
              client.println("Connection: close");
              client.println();
              status = true;
      }
     else
     {
      Serial.println("connection failed");
      }
  while(status)
      {
            if (client.available())   
                    {
                        char c = client.read(); //save http header to c
                        Serial.print(c); //print http header to serial monitor
                    }
             if (!client.connected()) 
                    {
                        Serial.println();
                        Serial.println("disconnecting.");
                        Serial.print("Data sent :");
                        client.stop();
                        status = false;
                        data = ""; //data reset
                    }
      }
}

void loop() 
{
  checkkeys();
  isObstacle = digitalRead(ir);
  if(isObstacle == HIGH)
  {
    //Serial.println("Obstacle!!, Obstacle!!");
    digitalWrite(buzz,HIGH);
    alert();
    delay(1000);  
  }
  else
  {
    digitalWrite(buzz,LOW);
  }



  if (!Serial.available())
  {
    //Serial.println("I see Nothing...");
  }
  else if(Serial.available())
  {
    //b=Serial.read();
    b="";
     while(Serial.available())
     {
         char t= Serial.read(); 
         b=b+t;
     }
    Serial.end();
    Serial.begin(115200);
    push();
    //Serial.println(b);
   // digitalWrite(LED, HIGH);
    //delay(100);          // wait for 1 second.
    //digitalWrite(LED, LOW); 
  }
}
