#include "LIB_Config.h"

extern uint32_t USART3_Count;
extern uint32_t USART2_Count;
extern uint32_t ADC_Count;
extern uint32_t TEMP_Count;
extern uint32_t RTC_Count;
extern uint32_t ACC_Count;
extern uint32_t JOY_Count;

uint8_t u8_buffer_index = 0;
#define BUFFER_SIZE 50
uint8_t u8_buffer[BUFFER_SIZE];
uint8_t resultString[BUFFER_SIZE] = {0};

JsonPair jsonPair;								//存放JSON对

void buffer_clear(void)
{
	int i;
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		u8_buffer[i] = 0x00;
		resultString[i] = 0x00;
	}
}

void control_device(JsonPair *jsonPair)
{
	//RGB LED
	if (!strcmp(jsonPair->key,"RGB"))
	{
		RGB rgb = parse_rgb(jsonPair->value);
		LED_RGB_Control(rgb.R,rgb.G,rgb.B);
		USART3_printf("R:%d,G:%d,B:%d\r\n",rgb.R,rgb.G,rgb.B);
	}
	
	//Buzzer joysticks
	if (!strcmp(jsonPair->key,"BZ"))
	{
		if (!strcmp(jsonPair->value,"on"))
		{
			printf("{\"BZ\":\"on\"}\r\n");//回复
			Buzzer_Control(1);
		}
		else if (!strcmp(jsonPair->value,"off"))
		{
			printf("{\"BZ\":\"off\"}\r\n");//回复
			Buzzer_Control(0);
		}
	}
	
	//OLED
	if (!strcmp(jsonPair->key,"OLED"))
	{
		if (strcmp(jsonPair->value,""))
		{
		USART3_printf("OLED=%s\r\n",jsonPair->value);
		ssd1306_clear_screen(0x00);
		ssd1306_display_string(30, 0, "Waveshare", 16, 1);
		ssd1306_display_string(0, 16, (uint8_t *)jsonPair->value, 16, 1);
		ssd1306_refresh_gram();
		}
		else
		{
			ssd1306_clear_screen(0x00);
		}
	}
}

void ADXL345_Printf()
{
	int16_t value[3];
	float ADXL345_Result[3];
	
	ADXL345_Get_Output(value);
	ADXL345_Data_Process(ADXL345_Result,value);
	
	printf("{\"ACC\":\"X=%.1f  Y=%.1f  Z=%.1f\"}\r\n", ADXL345_Result[0], ADXL345_Result[1], ADXL345_Result[2]);
}

void Joystick_Printf()
{
	static uint16_t Joystick_backup = 0;
	uint16_t Joystick_now = 0;

	Joystick_now = Joystick_Scan(1);
	
	if (Joystick_backup != Joystick_now)
	{
		Joystick_backup = Joystick_now;
		switch (Joystick_now)
		{
			//case 0:printf("{\"JOY\":\"No\"}\r\n");break;
			case 0:break;
			case 1:printf("{\"JOY\":\"Left\"}\r\n");break;
			case 2:printf("{\"JOY\":\"Up\"}\r\n");break;
			case 3:printf("{\"JOY\":\"Down\"}\r\n");break;
			case 4:printf("{\"JOY\":\"Right\"}\r\n");break;
			case 5:printf("{\"JOY\":\"Enter\"}\r\n");break;
		}
	}
}

void ADC_Printf()
{
	static uint16_t ADC_backup = 0;
	uint16_t ADC_now = 0;

	ADC_now = adc_get_average(ADC1, ADC_Channel_0, 10);

	if (ADC_backup != ADC_now)
	{
		ADC_backup = ADC_now;
		printf("{\"ADC\":\"%.3f\"}\r\n",((float)ADC_now/4096)*3.3);
		USART3_printf("{\"ADC\":\"%.3f\"}\r\n",((float)ADC_now/4096)*3.3);//!
		
	//	printf("{\"ADC\":\"%d\"}\r\n",ADC_now);
	//	USART3_printf("{\"ADC\":\"%d\"}\r\n",ADC_now);//!
	}
}

void TEMP_Printf()
{
	static float TEMP_backup = 0.0;
	float TEMP_now = 0.0;

	TEMP_now = getTemp();

	if (TEMP_now != TEMP_backup)
	{
		TEMP_backup = TEMP_now;
		printf("{\"TEMP\":\"%.3f\"}\r\n",TEMP_now);
	}
}

void RTC_Printf()
{
	static struct ts t;

	DS3231_Get(&t);
	printf("{\"RTC\":\"%d.%02d.%02d %02d:%02d:%02d\"}\r\n",t.year, t.mon, t.mday, t.hour, t.min, t.sec);
}

int main(void) 
{
	uint8_t u8_temp;
	uint32_t status = 0; //状态机
	struct ts t2; //!!!
	system_init();
	
	buffer_clear();
	USART3_printf("USART3 Init Success\r\n");
	
	t2.sec = 15;
	t2.min = 45;
	t2.hour = 12;
	t2.wday = 6;
	t2.mday = 8;
	t2.mon = 10;
	t2.year = 2016;
	DS3231_Set(t2);
	
	printf("{\"JOY\":\"No\"}\r\n");
	


	while (1) 
	{
		switch (status)
		{
			case 0:
				if (fifo_out(&u8_temp) != -1)
				{
					if ('{' == u8_temp)
					{
						buffer_clear();
						u8_buffer[0] = '{';
						status = 1;
						u8_buffer_index = 1;
					}
				}
				break;
			case 1:
				if (fifo_out(&u8_temp) != -1)
				{
					if('}' == u8_temp)
					{
						u8_buffer[u8_buffer_index] = u8_temp;
						if (get_json((char *)resultString,(char *)u8_buffer))
						{	
							if (parse_json((char *)resultString,&jsonPair))
							{
								printf_json(&jsonPair);
								control_device(&jsonPair);
							}
						}
						status = 0;
					}
					else
					{
						u8_buffer[u8_buffer_index++] = u8_temp;
					}
				}
				break;
		}
		
		if (USART2_Count > 100)
		{
			USART2_Count = 0;
		}
		
		if (JOY_Count > 100)
		{
			JOY_Count = 0;
			
			Joystick_Printf();
		}
		
		if (ADC_Count > 400)
		{	
			ADC_Count = 0;
			
			ADC_Printf();
		}
		
		if (TEMP_Count > 1000)
		{
			TEMP_Count = 0;
			
			TEMP_Printf();
		}
		
		if (RTC_Count > 1000)
		{
			RTC_Count = 0;
			
			RTC_Printf();
		}
		
		if (ACC_Count > 500)
		{
			ACC_Count = 0;
		
			ADXL345_Printf();
		}
	}
}


/*-------------------------------END OF FILE-------------------------------*/

