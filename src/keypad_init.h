#include "LCD.h"
#include "stm32l476xx.h"
#include "stdbool.h"
#ifndef __STM32L476R_NUCLEO_keypad_init_H
#define __STM32L476R_NUCLEO_keypad_init_H

void I2C_Initialization(I2C_TypeDef *I2C2);

void I2C_Start(I2C_TypeDef *I2C2, uint8_t DevAddress, uint8_t Size, uint8_t Direction);
void I2C_Stop(I2C_TypeDef *I2C2);
void I2C_WaitLineIdle(I2C_TypeDef *I2C2);
int8_t I2C_RecieveData(I2C_TypeDef *I2C2, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size);
int8_t I2C_SendData(I2C_TypeDef *I2C2, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size);

#endif