#include "ADC.h"

uint16_t adc_get_value(ADC_TypeDef* ADCx, uint8_t chChannel)	 
{
	uint8_t chTimeout = 0;

#ifdef STM32F10X_MD
	ADC_RegularChannelConfig(ADCx, chChannel, 1, ADC_SampleTime_239Cycles5 );	
  
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); 	
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) && (++chTimeout));

#elif defined(STM32F302x8) || defined(STM32F030) || defined(STM32F401xx)

	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC) && (++chTimeout));
	
#endif

	return ADC_GetConversionValue(ADCx);	
}

uint16_t adc_get_average(ADC_TypeDef* ADCx, uint8_t chChannel, uint8_t chTimes)
{
	uint16_t i;
	uint32_t chTemp = 0;
	
	for(i = 0; i < chTimes; ++i) 
	{
		chTemp += adc_get_value(ADCx, chChannel);
		delay_us(10);
	}
	chTemp /= chTimes;
	
	return chTemp;
} 
