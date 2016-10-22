#include "ADXL345.h"

int16_t ADXL345_Offset[3];


void ADXL345_Init(void)
{
	ADXL345_Set_POWER_CTL(0x00);
	delay_ms(10);

	ADXL345_Set_DATA_FORMAT(0x0B);
	delay_ms(10);

	ADXL345_Set_POWER_CTL(MeasurementMode);
	delay_ms(10);

	ADXL345_Offset[0] = 4;		//x
	ADXL345_Offset[1] = 10;		//y
	ADXL345_Offset[2] = -20;	//z
}

uint8_t ADXL345_Get_DEVID(void)
{
	return I2C_ReadOneByte(ADXL345_I2C_ADDRESS, ADXL345_DEVID_REG);
}

uint8_t ADXL345_Get_BW_RATE(void)
{
	return I2C_ReadOneByte(ADXL345_I2C_ADDRESS, ADXL345_BW_RATE_REG);
}

uint8_t ADXL345_Get_POWER_CTL(void)
{
	return I2C_ReadOneByte(ADXL345_I2C_ADDRESS, ADXL345_POWER_CTL_REG);
}

void ADXL345_Set_POWER_CTL(uint8_t Reg)
{
	I2C_WriteOneByte(ADXL345_I2C_ADDRESS, ADXL345_POWER_CTL_REG, Reg);
}

uint8_t ADXL345_Get_DATA_FORMAT(void)
{
	return I2C_ReadOneByte(ADXL345_I2C_ADDRESS, ADXL345_DATA_FORMAT_REG);
}

void ADXL345_Set_DATA_FORMAT(uint8_t Reg)
{
	I2C_WriteOneByte(ADXL345_I2C_ADDRESS, ADXL345_DATA_FORMAT_REG, Reg);
}

void ADXL345_Get_Output(int16_t* buffer)
{
	uint8_t temp[6];
	I2C_ReadBuff(ADXL345_I2C_ADDRESS, ADXL345_DATAX0_REG, 6, temp);
	
	 buffer[0] = (uint16_t)temp[1] << 8 | (uint16_t)temp[0];
   buffer[1] = (uint16_t)temp[3] << 8 | (uint16_t)temp[2];
   buffer[2] = (uint16_t)temp[5] << 8 | (uint16_t)temp[4];
}

void ADXL345_Data_Process(float* result, int16_t* buffer)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		result[i] = (buffer[i] - ADXL345_Offset[i]) * 0.004 * 9.80665;
	}
}

