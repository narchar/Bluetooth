#include "Buzzer.h"

void Buzzer_Init(void)
{
	BUZZER_OFF;
}

void Buzzer_Control(uint8_t State)
{
	State ? (BUZZER_ON):(BUZZER_OFF);
}

