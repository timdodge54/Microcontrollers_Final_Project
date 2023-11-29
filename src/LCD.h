#include "stm32l476xx.h"
#include "stdbool.h"

#ifndef __STM32L476R_NUCLEO_LCD_H
#define __STM32L476R_NUCLEO_LCD_H


void delay_ms(unsigned int ms);
void delay_micros(unsigned int micro_s); 
void LCD_WriteCom(unsigned char com);
void LCD_Init(void);
void LCD_Clear(void);
void LCD_DisplayString(unsigned int line_num, unsigned char *ptr);
void LCD_SendChar(unsigned char c);
void LCD_Pulse(void);

#endif
