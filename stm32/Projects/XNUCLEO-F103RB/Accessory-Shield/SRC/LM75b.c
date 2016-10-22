#include "LM75b.h"

static int m_u8I2CAddr;
static uint16_t getReg(LM75Register reg);
//static void setReg(LM75Register reg, uint16_t newValue);


void LM75b_Init(uint8_t i2c_addr)
{
	m_u8I2CAddr = (i2c_addr << 1);
}

uint16_t getReg(LM75Register reg)
{
	uint8_t c = reg;
	uint8_t buff[2];
	
	if (reg != Conf_reg)
	{
		I2C_ReadBuff(m_u8I2CAddr,c,2,buff);
		return ((buff[0] << 8) | buff[1]);
	}
	else
	{
		buff[0] = I2C_ReadOneByte(m_u8I2CAddr,c);
		return buff[0];
	}
}

float getTemp(void)
{
	return getTempReg()/256.0;
}

uint16_t getTempReg(void)
{
	return getReg(Temp_reg);
}

uint16_t getTHystReg(void)
{
	return getReg(THyst_reg);
}

uint16_t getTosReg(void)
{
	return getReg(Tos_reg);
}

uint16_t getConfReg(void)
{
	return getReg(Conf_reg);
}

//void setReg(LM75Register reg, uint16_t newValue)
//{
//	uint8_t buff[2];
//	buff[0] = (newValue >> 8) & 0xFF;
//	buff[1] = newValue & 0xFF;
//	
//	if (reg != Conf_reg)
//	{
//		I2C_WriteOneByte(m_u8I2CAddr,reg,buff[1]);
//	}
//	else
//	{
//		I2C_WriteBuff(m_u8I2CAddr,reg,2,buff);
//	}
//}

//void setTHystReg(uint16_t newValue)
//{
//	setReg(THyst_reg,newValue);
//}

