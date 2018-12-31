#include<Servo.h>
Servo myServo;
int pos = 0;
String b;
void setup() 
{
  Serial.begin(115200);
  // put your setup code here, to run once:
myServo.attach(4);
}
void openn()
{
  for (pos = 180; pos >= 0; pos -= 1)
      {
        myServo.write(pos); 
        delay(10);
       
      }
      delay(5000);
      for (pos = 0; pos <= 180; pos += 1) 
      {
        myServo.write(pos);
        delay(10); 
                     
      }
}

void loop() 
{
  // put your main code here, to run repeatedly:
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
    openn();
  }
}
