/*********************************************************************************************************
*
* File                : adc.ino
* Hardware Environment: 
* Build Environment   : Arduino
* Version             : V1.0.6
* By                  : WaveShare
*
*                                  (c) Copyright 2005-2014, WaveShare
*                                       http://www.waveshare.net
*                                       http://www.waveshare.com   
*                                          All Rights Reserved
*
*********************************************************************************************************/
#include <aJSON.h>

//JSON
aJsonStream serial_stream(&Serial);

int LED_Count = 0;

//电机速度
#define LEFT_LOW_SPEED      100
#define LEFT_MEDIUM_SPEED   140
#define LEFT_HIGH_SPEED     215

#define RIGHT_LOW_SPEED     100
#define RIGHT_MEDIUM_SPEED  180
#define RIGHT_HIGH_SPEED    255

//初始化电机相关引脚
#define LMotorEN  5
#define LMotorIN0 A0
#define LMotorIN1 A1
#define RMotorEN  6
#define RMotorIN0 A2
#define RMotorIN1 A3

//设置速度
char left_speed = LEFT_MEDIUM_SPEED;
char right_speed = RIGHT_MEDIUM_SPEED;

void MotorInit()
{
  //motorA
  pinMode(LMotorEN,  OUTPUT);     //ENA
  pinMode(LMotorIN0, OUTPUT);    //IN0
  pinMode(LMotorIN1, OUTPUT);    //IN1
  //motorB
  pinMode(RMotorEN,  OUTPUT);     //ENB
  pinMode(RMotorIN0, OUTPUT);    //IN3
  pinMode(RMotorIN1, OUTPUT);    //IN4
}

void LeftMotorForward(char Speed = LEFT_MEDIUM_SPEED)
{
  analogWrite(LMotorEN,Speed);
  digitalWrite(LMotorIN0,HIGH);
  digitalWrite(LMotorIN1,LOW);
}

void LeftMotorBackward(char Speed = LEFT_MEDIUM_SPEED)
{
  analogWrite(LMotorEN,Speed);
  digitalWrite(LMotorIN0,LOW);
  digitalWrite(LMotorIN1,HIGH);
}

void LeftMotorStop()
{
  analogWrite(LMotorEN,0);
  digitalWrite(LMotorIN0,LOW);
  digitalWrite(LMotorIN1,LOW); 
}

void RightMotorForward(char Speed = RIGHT_MEDIUM_SPEED)
{
  analogWrite(RMotorEN,Speed);
  digitalWrite(RMotorIN0,LOW);
  digitalWrite(RMotorIN1,HIGH);
}

void RightMotorBackward(char Speed = RIGHT_MEDIUM_SPEED)
{
  analogWrite(RMotorEN,Speed);
  digitalWrite(RMotorIN0,HIGH);
  digitalWrite(RMotorIN1,LOW);
}

void RightMotorStop()
{
  analogWrite(RMotorEN,0);
  digitalWrite(RMotorIN0,LOW);
  digitalWrite(RMotorIN1,LOW); 
}

void CarForward(char lspeed = LEFT_MEDIUM_SPEED, char rspeed = RIGHT_MEDIUM_SPEED)
{
  LeftMotorForward(lspeed);
  RightMotorForward(rspeed);
}

void CarBackward(char lspeed = LEFT_MEDIUM_SPEED, char rspeed = RIGHT_MEDIUM_SPEED)
{
  LeftMotorBackward(lspeed);
  RightMotorBackward(rspeed);
}

void CarLeft(char rspeed = RIGHT_MEDIUM_SPEED)
{
  LeftMotorStop();
  RightMotorForward(rspeed);
}

void CarRight(char lspeed = LEFT_MEDIUM_SPEED)
{
  LeftMotorForward(lspeed);
  RightMotorStop();
}

void CarStop()
{
  LeftMotorStop();
  RightMotorStop();
}

void setup(void)
{
  Serial.begin(115200);

  MotorInit();                    
  Serial.println("{\"State\":\"Waveshare\"}");
}

//{"Forward":"Down"}
//{"Forward":"Up"}
//{"Backward":"Down"}
//{"Backward":"Up"}
//{"Left":"Down"}
//{"Left":"Up"}
//{"Right":"Down"}
//{"Right":"Up"}
//{"Low":"Down"}
//{"Medium":"Down"}
//{"High":"Down"}
void ComExecution(aJsonObject *msg)
{
  aJsonObject *forward = aJson.getObjectItem(msg, "Forward");
  if (forward)
  {
     String str = forward->valuestring;
     if(str.equals("Down"))
     {
      CarForward(left_speed,right_speed);
     }
     else if (str.equals("Up"))
     {
      CarStop();
     }
  }

  aJsonObject *backward = aJson.getObjectItem(msg, "Backward");
  if (backward)
  {
     String str = backward->valuestring;
     if(str.equals("Down"))
     {
      CarBackward(left_speed,right_speed);
     }
     else if (str.equals("Up"))
     {
      CarStop();
     }
  }

  aJsonObject *left = aJson.getObjectItem(msg, "Left");
  if (left)
  {
     String str = left->valuestring;
     if(str.equals("Down"))
     {
       CarLeft(right_speed);
     }
     else if (str.equals("Up"))
     {
      CarStop();
     }
  }

  aJsonObject *right = aJson.getObjectItem(msg, "Right");
  if (right)
  {
     String str = right->valuestring;
     if(str.equals("Down"))
     {
      CarRight(left_speed);
     }
     else if (str.equals("Up"))
     {
      CarStop();
     }
  }

  aJsonObject *low = aJson.getObjectItem(msg, "Low");
  if (low)
  {
     String str = low->valuestring;
     if(str.equals("Down"))
     {
        left_speed = LEFT_LOW_SPEED;
        right_speed = RIGHT_LOW_SPEED;
     }
  }
  
  aJsonObject *medium = aJson.getObjectItem(msg, "Medium");
  if (medium)
  {
     String str = medium->valuestring;
     if(str.equals("Down"))
     {
      left_speed = LEFT_MEDIUM_SPEED;
      right_speed = RIGHT_MEDIUM_SPEED;
     }
  }

  aJsonObject *high = aJson.getObjectItem(msg, "High");
  if (high)
  {
     String str = high->valuestring;
     if(str.equals("Down"))
     {
      left_speed = LEFT_HIGH_SPEED;
      right_speed = RIGHT_HIGH_SPEED;
     }
  }

}

void loop(void)
{
  if (serial_stream.available())
  {
    serial_stream.skip();
  }
  
  if (serial_stream.available())
  {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}







