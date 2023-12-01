#include "LCD.h"

void pinMode(GPIO_TypeDef *port, unsigned int pin, unsigned int mode) {
    //port->MODER = ...................
	  port->MODER = (port->MODER & ~(0x3u << (pin*2))) | (mode << (pin*2));
}

void setOutputType(GPIO_TypeDef *port, unsigned int pin, unsigned int type) {
    //port->OTYPER = ...................
		port->OTYPER = (port->OTYPER & ~(0x1u << (pin))) | (type << (pin));
}

void setPullUpDown(GPIO_TypeDef *port, unsigned int pin, unsigned int pupd) {
    //port->PUPDR = ...................
		port->PUPDR = (port->PUPDR & ~(0x3u << (pin*2))) | (pupd << (pin*2));
}

void digitalWrite(GPIO_TypeDef *port, unsigned int pin, unsigned int value) {
    //port->ODR = ...................
		port->ODR = (port->ODR & ~(0x1u << (pin))) | (value << (pin));
}

unsigned int digitalRead(GPIO_TypeDef *port, unsigned int pin) {
    //return ...................
	  return ((port->IDR >> pin) & 0x1u);
}