#ifndef _ADC_H
#define _ADC_H

#include "LIB_Config.h"

uint16_t adc_get_value(ADC_TypeDef* ADCx, uint8_t chChannel);
uint16_t adc_get_average(ADC_TypeDef* ADCx, uint8_t chChannel, uint8_t chTimes);

#endif /*_ADC_H*/

