#include "stm32l476xx.h"
#include "keypad_init.h"
#include "stdint.h"
#ifndef __STM32L476R_NUCLEO_usart_H
#define __STM32L476R_NUCLEO_usart_H


uint8_t volatile static note_buffer;
uint8_t volatile static sharpen_flatten[3] = {0x30,0x30,0x30};
uint8_t volatile static sign_of_weight = ',';
uint32_t volatile static note_counter = 1;

void USART_Init(USART_TypeDef * USARTx);
void USART_drive(void);
void USART_Read(USART_TypeDef *USARTx, uint8_t * buffer, uint32_t nBytes);
void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes);
void ADC1_wakeup(void);
void TIM4_Init(void);
void ADC_init(void);
void UReceive(USART_TypeDef *USARTx, uint8_t * buffer, uint32_t nBytes);
void USART2_IRQHandler(void);
void buzzer_setup(void);
void buzzer(void);
#endif
