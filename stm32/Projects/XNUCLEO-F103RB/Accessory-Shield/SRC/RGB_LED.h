#ifndef _RGB_LED_H
#define _RGB_LED_H

#include "LIB_Config.h"

#define  R_MAX  255
#define  G_MAX  255
#define  B_MAX  255

#define SCL_LOW 	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET)
#define SCL_HIGH 	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET)

#define SDA_LOW		GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET)
#define SDA_HIGH	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET)

#define LED_EN		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET)

void RGB_LED_Init(void);
void LED_RGB_Control(uint8_t R,uint8_t G,uint8_t B);

#endif /*_RGB_LED_H*/

