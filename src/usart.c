#include "usart.h"




void USART_Init(USART_TypeDef * USARTx){
	// Disable Usart
	USARTx -> CR1 &= ~USART_CR1_UE;
	
	// Set data length to 8 bits
	USARTx->CR1 &= ~USART_CR1_M;
	
	// Select 1 stop bit
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	// Set parity as no parity
	USARTx->CR1 &= ~USART_CR1_PCE;

	// Oversampling by 16
	USARTx -> CR1 &= ~USART_CR1_OVER8;
	
	// Set Baud rate to 9600
	USARTx->BRR = 0x1A1u;
	
	//Enable transmission and reception
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	//Enable UART inturupts
	USARTx->CR1 |= USART_CR1_RXNEIE;
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);
	
	// Enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	// Verify that USART is ready
	while ((USARTx->ISR & USART_ISR_TEACK)==0);
	
	//Verify that USART is ready for reception
	while ((USARTx->ISR & USART_ISR_REACK) == 0);
}

void USART_drive(void){
	//-------GPIO Initialization for USART 1
	// PB.6 = 
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;// Enable GPIO port A clock
	
	GPIOA->MODER &= ~(0xFu << (2*2)); // clear mode bits for pin 6 and 7
	GPIOA->MODER |= 0xA << (2*2); //set Alternate function mode
	
	GPIOA->AFR[0] |= 0x77 << (4*2);// set pin 6 and 7 to AF 7
	GPIOA->OSPEEDR |= 0xF <<(2*2);// gipio speed 
	
	GPIOA->PUPDR &= ~(0xFu<<2*2);
	GPIOA->PUPDR |= 0x5<<(2*2); // select pullup
	GPIOA->OTYPER &= ~(0x3u<<2); // gpio output type
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR &= ~(RCC_CCIPR_USART2SEL);
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
	
	USART_Init(USART2);
}

void USART2_IRQHandler(void) {
	UReceive(USART2, &note_buffer, note_counter);
}

void UReceive(USART_TypeDef *USARTx, uint8_t * buffer, uint32_t nBytes){
	int i;
	for (i = 0; i < nBytes; ++i){
		if (USARTx->ISR & USART_ISR_RXNE){// Wait until hardware sets RXE
			buffer[i] = USARTx->RDR;
		} 
	}
}

void USART_Read(USART_TypeDef *USARTx, uint8_t * buffer, uint32_t nBytes){
	int i;
	for (i = 0; i < nBytes; ++i){
		while(!(USARTx->ISR & USART_ISR_RXNE)); // Wait until hardware sets RXE
		buffer[i] = USARTx->RDR;
		//if (buffer[i] == 't' | buffer[i] == 'T'){
			//uint8_t stfu[] = {'s','t','f','u'};
			//USART_Write(USART2, stfu, 4); 
	}
}

void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t nBytes){
	int i;
	for (i=0; i<nBytes; ++i){
		while (!(USARTx->ISR & USART_ISR_TXE)); // Wait until hardwar sets TXE
		USARTx->TDR = buffer[i] & 0xFF; // Writing to TDR clears TXE flag
	}
	
	// Wait until TC (transmission complete) bit is set
	while (!(USARTx->ISR & USART_ISR_TC));
	
	// Writing 1 to the TCCF (transmission complete clear flag) bit in ICR clears TC bit in ISR
	USARTx->ICR |= USART_ICR_TCCF;
}

void ADC_init(void){
	
	RCC->APB2ENR |= RCC_AHB2ENR_ADCEN;
	ADC1->CR &= ~(ADC_CR_ADEN);
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	ADC123_COMMON->CCR |= ADC_CCR_PRESC;
	ADC123_COMMON->CCR |= ADC_CCR_DUAL;
	ADC1_wakeup();
	ADC1->CFGR &= ~(ADC_CFGR_RES);
	ADC1->CFGR &= ~(ADC_CFGR_ALIGN);
	ADC1->SQR1 &= ~(ADC_SQR1_L);
	
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= (17U << 6);
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_17;
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP17;
	ADC1->SMPR2 &= ADC_SMPR2_SMP17;
	ADC1->SMPR2 &= ~(ADC_SMPR2_SMP17_0);
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	ADC1->CFGR &= ADC_CFGR_DISCEN;
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	ADC1->CR &= ADC_CR_ADEN;
	while ((ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY);
}


void ADC1_wakeup(void){
	int wait_time;
	if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD){
		ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power mode if in deep power mode
	}
	
	ADC1->CR |= ADC_CR_ADVREGEN;
	wait_time = 1600;
	while (wait_time != 0){
		wait_time--;
	}

}

void TIM4_Init(void){
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	TIM4->CR1 &= ~TIM_CR1_CMS;
	TIM4->CR1 &= ~TIM_CR1_DIR;
	
	TIM4->CR2 &= ~TIM_CR2_MMS;
	TIM4->CR2 |= ~TIM_CR2_MMS_2;
	
	TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
	TIM4->PSC = 7;
	TIM4->ARR = 999;
	TIM4->CCR1 = 500;
	
	TIM4->CCER |= TIM_CCER_CC1E;
	TIM4->CR1 |= TIM_CR1_CEN;
}

void buzzer(void)
{	
	// If the letter given is not in the switch statement, use A
	int note;
	switch (note_buffer) {
    case 'q':
        note = THIRD[0];
        break;
    case 'w':
        note = THIRD[1];
        break;
    case 'e':
        note = THIRD[2];
        break;
    case 'r':
        note = THIRD[3];
        break;
    case 't':
        note = THIRD[4];
        break;
    case 'y':
        note = THIRD[5];
        break;
    case 'u':
        note = THIRD[6];
        break;
    case 'a':
        note = FOURTH[0];
        break;
    case 's':
        note = FOURTH[1];
        break;
    case 'd':
        note = FOURTH[2];
        break;
    case 'f':
        note = FOURTH[3];
        break;
    case 'g':
        note = FOURTH[4];
        break;
    case 'h':
        note = FOURTH[5];
        break;
    case 'j':
        note = FOURTH[6];
        break;
    case 'z':
        note = FIFTH[0];
        break;
    case 'x':
        note = FIFTH[1];
        break;
    case 'c':
        note = FIFTH[2];
        break;
    case 'v':
        note = FIFTH[3];
        break;
    case 'b':
        note = FIFTH[4];
        break;
    case 'n':
        note = FIFTH[5];
        break;
    case 'm':
        note = FIFTH[6];
        break;
    // Add more cases if needed
    default:
        note = 0;
        break;
}

	for (int i = 0; i < note; ++i)
	{
		for (int j = 0; j < 9091; ++j)
		{
			continue;
		}
		if (i % 2 == 0)
		{
			digitalWrite(GPIOB, 0x0u, 0x1u);
		}
		else
		{
			digitalWrite(GPIOB, 0x0u, 0x0u);
		}
	}
	digitalWrite(GPIOB, 0x0u, 0x0u);
}

void buzzer_setup(void)
{
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	pinMode(GPIOB, 0, 0x1u);
	setOutputType(GPIOB, 0, 0x0u);
	digitalWrite(GPIOB, 0, 0x1u);
}

