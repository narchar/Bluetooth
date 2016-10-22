#ifndef _BUZZER_H
#define _BUZZER_H

#include "LIB_Config.h"

#define BUZZER_ON		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET)
#define BUZZER_OFF	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET)

void Buzzer_Init(void);
void Buzzer_Control(uint8_t State);

#endif /*_BUZZER_H*/

