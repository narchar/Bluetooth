#include "RGB_LED.h"

static void _led_delay()
{
	volatile unsigned int i=0;
  for(i=0;i<40;i++);
}

static void _frame_head(void)
{
  unsigned char i; 
	SDA_LOW;   
	for (i=0; i<32; i++)
	{
		SCL_LOW;    	
		_led_delay();
		SCL_HIGH;    
		_led_delay(); 
	}
}

/********* invert the grey value of the first two bits ***************/
static uint8_t TakeAntiCode(uint8_t dat)
{
  uint8_t tmp = 0;

	tmp=((~dat) & 0xC0)>>6;		 
	return tmp;
}

/****** send gray data *********/
static void DatSend(uint32_t dx)
{
  uint8_t i;
 
	for (i=0; i<32; i++)
	{
	  if ((dx & 0x80000000) != 0)
		{
			SDA_HIGH;
		}
		else
		{
			SDA_LOW;
		}
 
		dx <<= 1;
		
		SCL_LOW;    	
		_led_delay();
		SCL_HIGH;    
		_led_delay(); 
	}
}
 
/******* data processing  ********************/
static void DataDealWithAndSend(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t dx = 0;

	dx |= (uint32_t)0x03 << 30; // The front of the two bits 1 is flag bits
	dx |= (uint32_t)TakeAntiCode(b) << 28;
	dx |= (uint32_t)TakeAntiCode(g) << 26;	
	dx |= (uint32_t)TakeAntiCode(r) << 24;

	dx |= (uint32_t)b << 16;
	dx |= (uint32_t)g << 8;
	dx |= r;

	DatSend(dx);
}

void RGB_LED_Init(void)
{
	LED_EN;
	SCL_LOW;	
	SDA_LOW;
 	_frame_head();	
	DataDealWithAndSend(0,0,0);	  
	DataDealWithAndSend(0,0,0);	  
}

void LED_RGB_Control(uint8_t R, uint8_t G, uint8_t B)
{
	_frame_head();
	DataDealWithAndSend(R, G, B);	 
	DataDealWithAndSend(R, G, B);	 
}
