#include "stm32l476xx.h"
#include "stdbool.h"

#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H


void pinMode(GPIO_TypeDef *port, unsigned int pin, unsigned int mode);

void setOutputType(GPIO_TypeDef *port, unsigned int pin, unsigned int type);

void setPullUpDown(GPIO_TypeDef *port, unsigned int pin, unsigned int pupd);

void digitalWrite(GPIO_TypeDef *port, unsigned int pin, unsigned int value);

unsigned int digitalRead(GPIO_TypeDef *port, unsigned int pin);
#endif
