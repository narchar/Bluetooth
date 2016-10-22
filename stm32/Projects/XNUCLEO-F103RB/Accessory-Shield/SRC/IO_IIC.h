#ifndef _IO_IIC_H
#define _IO_IIC_H

#include "LIB_Config.h"

#define I2C_SCL_PIN  GPIO_Pin_8
#define I2C_SDA_PIN  GPIO_Pin_9

#define I2C_SCL_Set()  GPIO_WriteBit(GPIOB, I2C_SCL_PIN, Bit_SET)
#define I2C_SCL_Clr()  GPIO_WriteBit(GPIOB, I2C_SCL_PIN, Bit_RESET)

#define I2C_SDA_Set()  GPIO_WriteBit(GPIOB, I2C_SDA_PIN, Bit_SET)
#define I2C_SDA_Clr()  GPIO_WriteBit(GPIOB, I2C_SDA_PIN, Bit_RESET)

#define I2C_SDA_Get()  GPIO_ReadInputDataBit(GPIOB, I2C_SDA_PIN)

#ifndef I2C_Direction_Transmitter
	#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#endif

#ifndef I2C_Direction_Receiver
	#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#endif

enum
{
	I2C_SDA_IN,
	I2C_SDA_OUT
};

enum
{
	I2C_ACK,
	I2C_NACK
};

void I2C_SDAMode(uint8_t Mode);
void I2C_Start(void);
void I2C_Stop(void);
bool I2C_WaiteForAck(void);
void I2C_Ack(void);
void I2C_NAck(void);
bool I2C_WriteOneBit(uint8_t DevAddr, uint8_t RegAddr, uint8_t BitNum, uint8_t Data);
bool I2C_WriteBits(uint8_t DevAddr, uint8_t RegAddr, uint8_t BitStart, uint8_t Length, uint8_t Data);
void I2C_WriteByte(uint8_t Data);
uint8_t I2C_ReadByte(uint8_t Ack);
void I2C_WriteOneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t Data);
uint8_t I2C_ReadOneByte(uint8_t DevAddr, uint8_t RegAddr);
bool I2C_WriteBuff(uint8_t DevAddr, uint8_t RegAddr, uint8_t Num, uint8_t *pBuff);
bool I2C_ReadBuff(uint8_t DevAddr, uint8_t RegAddr, uint8_t Num, uint8_t *pBuff);

void I2Cx_Init(void);

#endif	//_IO_IIC_H
