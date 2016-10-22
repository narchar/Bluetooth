#ifndef _SSD1306_H
#define _SSD1306_H

#include "LIB_Config.h"

//#define SH1106
#define SSD1306

//#define INTERFACE_3WIRE_SPI   //3-wire SPI 
//#define INTERFACE_4WIRE_SPI     //4-wire SPI 
#define INTERFACE_IIC         //I2C

#define SSD1306_CS_PIN          GPIO_Pin_12
#define SSD1306_CLK_PIN         GPIO_Pin_8//
#define SSD1306_DIN_PIN         GPIO_Pin_9//
#define SSD1306_RES_PIN         GPIO_Pin_8//
#define SSD1306_DC_PIN          GPIO_Pin_9//

#define SSD1306_CS_GPIO         GPIOB
#define SSD1306_CLK_GPIO        GPIOB//
#define SSD1306_DIN_GPIO        GPIOB//
#define SSD1306_RES_GPIO        GPIOA//
#define SSD1306_DC_GPIO         GPIOA// 

#define __SSD1306_CS_SET()      GPIO_WriteBit(SSD1306_CS_GPIO, SSD1306_CS_PIN, Bit_SET)
#define __SSD1306_CS_CLR()      GPIO_WriteBit(SSD1306_CS_GPIO, SSD1306_CS_PIN, Bit_RESET)

#define __SSD1306_RES_SET()     GPIO_WriteBit(SSD1306_RES_GPIO, SSD1306_RES_PIN, Bit_SET)
#define __SSD1306_RES_CLR()     GPIO_WriteBit(SSD1306_RES_GPIO, SSD1306_RES_PIN, Bit_RESET)

#define __SSD1306_DC_SET()      GPIO_WriteBit(SSD1306_DC_GPIO, SSD1306_DC_PIN, Bit_SET)
#define __SSD1306_DC_CLR()      GPIO_WriteBit(SSD1306_DC_GPIO, SSD1306_DC_PIN, Bit_RESET)

#define __SSD1306_CLK_SET()     GPIO_WriteBit(SSD1306_CLK_GPIO, SSD1306_CLK_PIN, Bit_SET)
#define __SSD1306_CLK_CLR()     GPIO_WriteBit(SSD1306_CLK_GPIO, SSD1306_CLK_PIN, Bit_RESET)

#define __SSD1306_DIN_SET()     GPIO_WriteBit(SSD1306_DIN_GPIO, SSD1306_DIN_PIN, Bit_SET)
#define __SSD1306_DIN_CLR()     GPIO_WriteBit(SSD1306_DIN_GPIO, SSD1306_DIN_PIN, Bit_RESET)

#define IIC_SCL_PIN         GPIO_Pin_8//
#define IIC_SDA_PIN         GPIO_Pin_9//

#define IIC_SCL_GPIO        GPIOB
#define IIC_SDA_GPIO        GPIOB

#define __IIC_SCL_SET()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_SET)
#define __IIC_SCL_CLR()     GPIO_WriteBit(IIC_SCL_GPIO, IIC_SCL_PIN, Bit_RESET)

#define __IIC_SDA_SET()		GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_SET)
#define __IIC_SDA_CLR()     GPIO_WriteBit(IIC_SDA_GPIO, IIC_SDA_PIN, Bit_RESET)

#define __IIC_SDA_IN()     	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_IPU; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)				

#define __IIC_SDA_OUT() 	do { \
								GPIO_InitTypeDef tGPIO; \
								tGPIO.GPIO_Pin = IIC_SDA_PIN; \
								tGPIO.GPIO_Speed = GPIO_Speed_50MHz; \
								tGPIO.GPIO_Mode = GPIO_Mode_Out_PP; \
								GPIO_Init(IIC_SDA_GPIO, &tGPIO); \
							}while(0)   


#define __IIC_SDA_READ()    GPIO_ReadInputDataBit(IIC_SDA_GPIO, IIC_SDA_PIN)

void ssd1306_clear_screen(uint8_t chFill);
void ssd1306_refresh_gram(void);
void ssd1306_draw_point(uint8_t chXpos, uint8_t chYpos, uint8_t chPoint);
void ssd1306_fill_screen(uint8_t chXpos1, uint8_t chYpos1, uint8_t chXpos2, uint8_t chYpos2, uint8_t chDot);
void ssd1306_display_char(uint8_t chXpos, uint8_t chYpos, uint8_t chChr, uint8_t chSize, uint8_t chMode);
void ssd1306_display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen,uint8_t chSize);
void ssd1306_display_string(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchString, uint8_t chSize, uint8_t chMode);
void ssd1306_draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
void ssd1306_draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar);
void ssd1306_draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight);

void ssd1306_init(void);

#endif /*_SSD1306_H*/

