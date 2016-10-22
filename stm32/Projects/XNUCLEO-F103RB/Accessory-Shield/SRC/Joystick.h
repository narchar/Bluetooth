#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include "LIB_Config.h"

#define JOYA  	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define JOYB  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define JOYC  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define JOYD  	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define JOYCTR  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)

//void Joystick_Init(void);
uint8_t Joystick_Scan(uint8_t mode);

#endif /*_JOYSTICK_H*/

