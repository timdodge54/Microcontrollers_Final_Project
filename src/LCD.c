#include "LCD.h"
#define E_OFFSET 5

void delay_ms(unsigned int ms) {
	volatile unsigned int i, j;
	for (i = 0; i < ms; ++i){
		for (j = 0; j < 800; ++j){
			continue;
		}
	}
}

void delay_micros(unsigned int micro_s) {
	volatile unsigned int i, j;
	for (i = 0; i < micro_s; ++i){
		for (j = 0; j < 20; ++j){
			continue;
		}
	}
}


void LCD_WriteCom(unsigned char com) {
	volatile unsigned char com1, com2;
	com2 = com&0x0f;
	com1 = (com>>4)&0x0f;
	delay_ms(1);
	GPIOB->ODR |= ~(1<< E_OFFSET);
	delay_micros(1);
	GPIOB->ODR &= 0xFFFFFFEF;
	GPIOB->ODR &= 0xFFFFFF0F;
	GPIOB->ODR |= com1;
	LCD_Pulse();
	GPIOB->ODR &= 0xFFFFFF0F;
	GPIOB->ODR |= com2;
	LCD_Pulse();
	GPIOB->ODR &= 0xFFFFFFEF;
	delay_micros(40);
	LCD_Pulse();
}

void LCD_Pulse(void){
	GPIOB->ODR |= 1<< E_OFFSET;
	delay_ms(1);
		GPIOB->ODR &= ~(1<< E_OFFSET);
	delay_ms(1);
}


void LCD_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOAEN;
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	GPIOB->MODER &= 0xFFFFFF000;
	GPIOB->MODER |= 0x00000555;
	LCD_Clear();
	delay_ms(2);
	LCD_WriteCom(0x28);
	delay_micros(40);
	LCD_WriteCom(0x02);
	delay_micros(40);
	LCD_WriteCom(0x0C);
	delay_micros(40);
	LCD_WriteCom(0x06);
	delay_micros(40);
	LCD_WriteCom(0x0F);
	delay_ms(5);
}

void LCD_Clear(void){
	unsigned char reset = 0x01;
  LCD_WriteCom(reset);
}

void LCD_DisplayString(unsigned int line_num, unsigned char *ptr) {
	unsigned char i;
	if (line_num == 0){
	LCD_WriteCom(0x80);
	}
	else{
		LCD_WriteCom(0x80 + 0x30);
	}
	for (i=0;i <16; ++i){
		LCD_SendChar(ptr[i]);
	}

	
}

void LCD_SendChar(unsigned char c){ 
	GPIOB->ODR |= ~(1<< E_OFFSET);
	delay_micros(40);
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= c & 0x0f;
	LCD_Pulse();
	GPIOB->ODR = GPIOB->ODR;
	GPIOB->ODR &= 0xFFFFFFF0;
	GPIOB->ODR |= c >> 4 & 0x0f;
	LCD_Pulse();
}
