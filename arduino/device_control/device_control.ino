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
#include <MsTimer2.h> 
#include <Wire.h>
#include <ADXL345.h>
#include "ds3231.h"
#include <lm75.h>
#include <ChainableLED.h> //RGB
#include <aJSON.h>

const int analog_pin = 0;   //ADC输入引脚
#define    KEY_UP        A1
#define    KEY_DOWN      A5
#define    KEY_LEFT      A3
#define    KEY_RIGHT     A2
#define    KEY_ENTER     A4


ADXL345 accelerometer;     
TempI2C_LM75 termo = TempI2C_LM75(0x48,TempI2C_LM75::nine_bits);

//RGB
#define NUM_LEDS  1
const int rgb_pwr = 12;                                     //rgb power control
const int clk_pin = 6;                                      //clock
const int data_pin = 5;                                     //data
//const int analog_pin = 0;                                   //pot input
ChainableLED leds(clk_pin, data_pin, rgb_pwr, NUM_LEDS);

//JSON
aJsonStream serial_stream(&Serial);

int key_release_flag = 1; //按键状态
char buff[128];           //串口输出buffer
char serial_temp;    
char in_buff[128];        //串口输入buffer
  
int LED_Count = 0;
int ADC_Count = 0;
int ACC_Count = 0;
int RTC_Count = 0;
int JOY_Count = 0;
int TEMP_Count = 0;
int BZ_Count = 0;
int BZ_State = 0;

void setup(void)
{
  Serial.begin(115200);

  pinMode(9, INPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
    
  MsTimer2::set(1, sys_tick);     // 中断设置函数，每 1ms 进入一次中断
  MsTimer2::start();              //开始计时

  Wire.begin();                 
  DS3231_init(DS3231_INTCN);      //DS3231

  leds.pwr_set(PWR_ENABLE);                                 //open the rgb power supply
  leds.setColorRGB(0,0,0,0);
  
  if (!accelerometer.begin())    //初始化ADXL345
  {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    delay(500);
  }

  pinMode(KEY_UP, INPUT);
  digitalWrite(KEY_UP, HIGH);
  pinMode(KEY_DOWN, INPUT);
  digitalWrite(KEY_DOWN, HIGH);
  pinMode(KEY_LEFT, INPUT);
  digitalWrite(KEY_LEFT, HIGH);
  pinMode(KEY_RIGHT, INPUT);
  digitalWrite(KEY_RIGHT, HIGH);
  pinMode(KEY_ENTER, INPUT);
  digitalWrite(KEY_ENTER, HIGH);

  Serial.println("OK");
}

void sys_tick()                        //中断处理函数
{           
  ++LED_Count;             
  ++ADC_Count;
  ++ACC_Count;
  ++RTC_Count;
  ++JOY_Count;
  ++TEMP_Count;
  ++BZ_Count;
}

void led_tick()
{
//  static boolean output = HIGH;
//  digitalWrite(13, output);
//  output = !output; 
  digitalWrite(13,digitalRead(9));
}

void adc_tick()
{
  int adcvalue;
  adcvalue = analogRead(analog_pin);
  
  Serial.print("{\"ADC\":\"");
  Serial.print(adcvalue);
  Serial.print("\"}");
}

void acc_tick()
{
  Vector norm = accelerometer.readNormalize();

  Serial.print("{\"ACC\":\"");
  Serial.print(norm.XAxis);
  Serial.print(",");
  Serial.print(norm.YAxis);
  Serial.print(",");
  Serial.print(norm.ZAxis);
  Serial.print("\"}");
}

void rtc_tick()
{
  struct ts t;
  DS3231_get(&t);
  
  snprintf(buff, 128, "{\"RTC\":\"%d.%02d.%02d %02d:%02d:%02d\"}", t.year,
      t.mon, t.mday, t.hour, t.min, t.sec);
  
  Serial.print(buff);
}

void joy_tick()
{
  if((digitalRead(KEY_UP) == LOW) || (digitalRead(KEY_DOWN) == LOW) || (digitalRead(KEY_LEFT) == LOW) || (digitalRead(KEY_RIGHT) == LOW) || (digitalRead(KEY_ENTER) == LOW))
  {
    if(key_release_flag)
    {
      delay(10);
      
      if((digitalRead(KEY_UP) == LOW) || (digitalRead(KEY_DOWN) == LOW) || (digitalRead(KEY_LEFT) == LOW) || (digitalRead(KEY_RIGHT) == LOW) || (digitalRead(KEY_ENTER) == LOW))
      {
        key_release_flag = 0;
        if(digitalRead(KEY_UP) == LOW)
        {
          snprintf(buff, 128, "{\"JOY\":\"UP\"}");
          Serial.print(buff);
        }
        else if(digitalRead(KEY_DOWN) == LOW)
        {
          snprintf(buff, 128, "{\"JOY\":\"DOWN\"}");
          Serial.print(buff);
        }
        else if(digitalRead(KEY_LEFT) == LOW)
        {
          snprintf(buff, 128, "{\"JOY\":\"LEFT\"}");
          Serial.print(buff);
        }
        else if(digitalRead(KEY_RIGHT) == LOW)
        {
          snprintf(buff, 128, "{\"JOY\":\"RIGHT\"}");
          Serial.print(buff);
        }
        else if(digitalRead(KEY_ENTER) == LOW)
        {
          snprintf(buff, 128, "{\"JOY\":\"ENTER\"}");
          Serial.print(buff);
        }
      }
    }
  }
  else
  {
    key_release_flag = 1;
  }
}

void temp_tick()
{
  Serial.print("{\"TEMP\":\"");
  Serial.print(termo.getTemp());
  Serial.print("\"}");
}

void run_tick()
{
   //LED任务
  if (LED_Count > 100)
  {
    LED_Count = 0;
    led_tick();
  }

  if (TEMP_Count > 1000)
  {
    TEMP_Count = 0;
    temp_tick();
  }
  
  //ADC任务
  if (ADC_Count > 500)
  {
    ADC_Count = 0;
    adc_tick();
  }

  //ACC任务
  if (ACC_Count > 1000)
  {
    ACC_Count = 0;
    acc_tick();
  }
  
  if (JOY_Count > 100)
  {
    JOY_Count = 0;
    joy_tick();
  }

   if (RTC_Count > 1000)
   {
    RTC_Count = 0;
    rtc_tick();
   }

   if ((BZ_Count > 200) && (BZ_State))
   {
      if (BZ_State==1)
      {
        BZ_State = 0;
        
        snprintf(buff, 128, "{\"BZ\":\"on\"}");
        Serial.print(buff);
      }
      else if(BZ_State==2)
      {
        BZ_State = 0;
        
        snprintf(buff, 128, "{\"BZ\":\"off\"}");
        Serial.print(buff);
      }
   }
}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *buzzer = aJson.getObjectItem(msg, "BZ");
  if (buzzer)
  {
     String str = buzzer->valuestring;
     if(str.equals("on"))//{"BZ":"on"}
     {
        digitalWrite(11, 1);
        BZ_State = 1;
        BZ_Count = 0;
     }
     else if (str.equals("off"))//{"BZ":"off"} 
     {
        digitalWrite(11, 0);
        BZ_State = 2;
        BZ_Count = 0;
     }
  }

  aJsonObject *rgb = aJson.getObjectItem(msg, "RGB");
  if (rgb)
  {
    byte R,G,B;
    String str = rgb->valuestring;
    int temp = str.indexOf(',');
    int temp2 = str.lastIndexOf(',');
    R = str.substring(0, temp).toInt(); 
    G = str.substring(temp+1,temp2).toInt();
    B = str.substring(temp2+1,str.length()).toInt();

    Serial.print(" R:");
    Serial.print(R);
    Serial.print(" G:");
    Serial.print(G);
    Serial.print(" B:");
    Serial.println(B);

    leds.setColorRGB(0,R,G,B);
  }
}

void loop(void)
{
 run_tick();

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







