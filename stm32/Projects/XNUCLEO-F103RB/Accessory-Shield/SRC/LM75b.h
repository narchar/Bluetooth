#ifndef __LM75B_H
#define __LM75B_H

#include "LIB_Config.h"

typedef enum 
{
	nine_bits = 0, 
	ten_bits, 
	eleven_bits, 
	twelve_bits 
} Resolution;

typedef enum 
{
	comparator_mode=0, 
	interrupt_mode 
} TermostatMode;

typedef enum 
{
	one_samples = 0, 
	two_samples, 
	four_samples, 
	six_samples 
} TermostatFaultTolerance;

typedef enum 
{
	active_low = 0, 
	active_high 
} OSPolarity;

typedef enum  
{
	Temp_reg=0, 	// read only
	Conf_reg, 		// R/W				POR state:00h
	THyst_reg, 		// R/W				POR state:4B00h
	Tos_reg 			// R/W				POR state:5000h
} LM75Register;

void LM75b_Init(uint8_t i2c_addr);
float getTemp(void);

uint16_t getTempReg(void);
uint16_t getConfReg(void);
uint16_t getTHystReg(void);
uint16_t getTosReg(void);

//void setTHystReg(uint16_t newValue);



#endif	//__LM75B_H
