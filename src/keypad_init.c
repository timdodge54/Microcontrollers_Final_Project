#include "keypad_init.h"

void pinMode(GPIO_TypeDef *port, unsigned int pin, unsigned int mode)
{
	// port->MODER = ...................
	port->MODER = (port->MODER & ~(0x3u << (pin * 2))) | (mode << (pin * 2));
}

void setOutputType(GPIO_TypeDef *port, unsigned int pin, unsigned int type)
{
	// port->OTYPER = ...................
	port->OTYPER = (port->OTYPER & ~(0x1u << (pin))) | (type << (pin));
}

void setPullUpDown(GPIO_TypeDef *port, unsigned int pin, unsigned int pupd)
{
	// port->PUPDR = ...................
	port->PUPDR = (port->PUPDR & ~(0x3u << (pin * 2))) | (pupd << (pin * 2));
}

void digitalWrite(GPIO_TypeDef *port, unsigned int pin, unsigned int value)
{
	// port->ODR = ...................
	port->ODR = (port->ODR & ~(0x1u << (pin))) | (value << (pin));
}

unsigned int digitalRead(GPIO_TypeDef *port, unsigned int pin)
{
	// return ...................
	return ((port->IDR >> pin) & 0x1u);
}

void I2C_Initialization(I2C_TypeDef *I2Cx)
{
	uint32_t OwnAddr = 0x52;			  // Own address of the I2C peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; // Enable clock for I2C1

	// 00 = PCLK, 01 = SYSCLK, 10 = HSI16, 11 = reserved
	RCC->CCIPR &= ~RCC_CCIPR_I2C1SEL;
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_0; // Select SYSCLK as clock source for I2C1

	RCC->APB1ENR1 |= RCC_APB1RSTR1_I2C1RST;	  // 1 = Reset I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST; // Complete the reset of I2C1

	// I2C CR1 Configuration
	// When the I2C is disabled (PE=0), the I2C performs a software reset
	I2Cx->CR1 &= ~I2C_CR1_PE; // 0 = Peripheral disabled
	I2Cx->CR1 &= ~I2C_CR1_ANFOFF;
	I2Cx->CR1 &= ~I2C_CR1_DNF;
	I2Cx->CR1 |= ~I2C_CR1_ERRIE;
	I2Cx->CR1 &= ~I2C_CR1_SMBHEN;
	I2Cx->CR1 &= ~I2C_CR1_NOSTRETCH;

	I2Cx->TIMINGR = 0;

	I2Cx->TIMINGR &= ~I2C_TIMINGR_PRESC;
	I2Cx->TIMINGR |= 7U << 28; // 7U = 0x7 = 0111
	I2Cx->TIMINGR |= 49U;
	I2Cx->TIMINGR |= 49U << 8;
	I2Cx->TIMINGR |= 14U << 20;
	I2Cx->TIMINGR |= 15U << 16;

	I2Cx->OAR1 &= ~I2C_OAR1_OA1EN;
	I2Cx->OAR1 |= I2C_OAR1_OA1EN | OwnAddr;
	I2Cx->OAR1 &= ~I2C_OAR2_OA2EN;

	I2Cx->CR2 &= ~I2C_CR2_ADD10;
	I2Cx->CR2 &= I2C_CR2_AUTOEND;
	I2Cx->CR2 &= ~I2C_CR2_NACK;
	I2Cx->CR2 &= ~I2C_CR2_RELOAD;
}

void I2C_Start(I2C_TypeDef *I2Cx, uint8_t DevAddress, uint8_t Size, uint32_t Direction)
{
	// Direction = 0: Master requests a write transfer
	// Direction = 1: Master requests a read transfer

	uint32_t tmpreg = I2Cx->CR2;
	tmpreg &= (uint32_t) ~((uint32_t)((I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP)));
	if (Direction == I2C_ISR_DIR)
	{
		tmpreg |= I2C_CR2_RD_WRN; // Read from slave
	}
	else
	{
		tmpreg &= ~I2C_CR2_RD_WRN; // Write to slave
	}
	tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16) & I2C_CR2_NBYTES));
		tmpreg |= I2C_CR2_START;
	I2Cx->CR2 = tmpreg;
}
void I2C_Stop(I2C_TypeDef *I2Cx)
{
	I2Cx->CR2 |= I2C_CR2_STOP;
	while ((I2Cx->ISR & I2C_ISR_STOPF) == 0)
		;
	I2Cx->ICR |= I2C_ICR_STOPCF; // Write 1 to clear STOPF flag in I2C_ICR register
}

void I2C_WaitLineIdle(I2C_TypeDef *I2Cx)
{
	while ((I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY)
		;
}
int8_t I2C_RecieveData(I2C_TypeDef *I2Cx, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size)
{
	int i;
	if (Size <= 0 || pData == NULL)
	{
		return -1;
	}
	I2C_WaitLineIdle(I2Cx);

	I2C_Start(I2Cx, SlaveAddress, Size, 1);

	for (i = 0; i < Size; ++i)
	{
		while ((I2Cx->ISR & I2C_ISR_RXNE) == 0)
			;
		pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	while ((I2Cx->ISR & I2C_ISR_TC) == 0)
		;

	I2C_Stop(I2Cx);
	return 0;
}

int8_t I2C_SendData(I2C_TypeDef *I2Cx, uint8_t SlaveAddress, uint8_t *pData, uint8_t Size)
{
	int i;
	if (Size <= 0 || pData == NULL)
	{
		return -1;
	}
	I2C_WaitLineIdle(I2Cx);

	I2C_Start(I2Cx, SlaveAddress, Size, 0);

	for (i = 0; i < Size; ++i)
	{
		while ((I2Cx->ISR & I2C_ISR_TXIS) == 0)
			;
		I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;
	}

	while ((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0)
		;
	if ((I2Cx->ISR & I2C_ISR_NACKF) != 0)
	{
		return -1;
	}
	I2C_Stop(I2Cx);
	return 0;
}

void keypad_main(void)
{
	// system clock init
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->CR |= RCC_CR_HSION;
	while (RCC->CR && RCC_CR_HSIRDY == 0)
		;
	RCC->CFGR &= RCC_CFGR_SW_HSI;

	pinMode(GPIOB, 6, 0x10);
	pinMode(GPIOB, 7, 0x10);
	setPullUpDown(GPIOB, 6, 0x1);
	setPullUpDown(GPIOB, 7, 0x1);
	setOutputType(GPIOB, 6, 0x1);
	setOutputType(GPIOB, 7, 0x1);
}
