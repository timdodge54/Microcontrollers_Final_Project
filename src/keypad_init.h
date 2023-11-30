#include "stm32l476xx.h"
#include "stdbool.h"
#include "stddef.h"
#ifndef __STM32L476R_NUCLEO_keypad_init_H
#define __STM32L476R_NUCLEO_keypad_init_H

void I2C_Initialization(I2C_TypeDef *I2Cx);

void I2C_Start(I2C_TypeDef *I2Cx, uint8_t DevAddress, uint8_t Size, uint32_t Direction);
void I2C_Stop(I2C_TypeDef *I2Cx);
void I2C_WaitLineIdle(I2C_TypeDef *I2Cx);
int8_t I2C_RecieveData(I2C_TypeDef *I2Cx, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size);
int8_t I2C_SendData(I2C_TypeDef *I2Cx, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size);
void pinMode(GPIO_TypeDef *port, unsigned int pin, unsigned int mode);

void setOutputType(GPIO_TypeDef *port, unsigned int pin, unsigned int type);

void setPullUpDown(GPIO_TypeDef *port, unsigned int pin, unsigned int pupd);

void digitalWrite(GPIO_TypeDef *port, unsigned int pin, unsigned int value);

unsigned int digitalRead(GPIO_TypeDef *port, unsigned int pin);
void keypad_main(void);

#endif