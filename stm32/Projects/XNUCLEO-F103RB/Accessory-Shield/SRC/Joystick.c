#include "Joystick.h"

uint8_t Joystick_Scan(uint8_t mode)
{
	static uint8_t Joystick_up = 1;
	if (mode) 
	{
		Joystick_up = 1;
	}
	
	if (Joystick_up&&((JOYA==0)||(JOYB==0)||(JOYC==0)||(JOYD==0)||(JOYCTR==0)))
	{
		delay_ms(10);
		Joystick_up = 0;
		if (JOYA == 0) return 1;
		else if (JOYB == 0) return 2;
		else if (JOYC == 0) return 3;
		else if (JOYD == 0) return 4;
		else if (JOYCTR == 0) return 5;
	}
	else if ((JOYA==1)||(JOYB==1)||(JOYC==1)||(JOYD==1)||(JOYCTR==1))
		Joystick_up = 1;
	return 0;
}

//void Joystick_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	//JOYA
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;				
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
//  NVIC_Init(&NVIC_InitStructure); 
//	
//	//JOYB
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;				
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
//  NVIC_Init(&NVIC_InitStructure); 
//	
//	//JOYC
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource0);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
//  NVIC_Init(&NVIC_InitStructure); 
//	
//	//JOYD
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;				
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
//  NVIC_Init(&NVIC_InitStructure); 
//	
//	//JOYCTR
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);

//	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;				
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
//  NVIC_Init(&NVIC_InitStructure); 
//	
//	printf("{\"JOY\":\"NO\"}\r\n");
//}

//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);
//	if(JOYA==0)	 	
//	{				 
//		printf("{\"JOY\":\"JOYA\"}\r\n");
//	}
//	if(JOYC==0)	 	
//	{				 
//		printf("{\"JOY\":\"JOYC\"}\r\n");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0); 
//}

//void EXTI1_IRQHandler(void)
//{
//	delay_ms(10);
//	if(JOYB==0)	 	
//	{				 
//		printf("{\"JOY\":\"JOYB\"}\r\n");
//		EXTI_ClearITPendingBit(EXTI_Line1); 
//	}
//	if(JOYCTR==0)	 	
//	{				 
//		printf("{\"JOY\":\"JOYCTR\"}\r\n");
//		EXTI_ClearITPendingBit(EXTI_Line1); 
//	}
//}

//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);
//	if(JOYD==0)	 	
//	{				 
//		printf("{\"JOY\":\"JOYD\"}\r\n");
//	}
//	EXTI_ClearITPendingBit(EXTI_Line4); 
//}
