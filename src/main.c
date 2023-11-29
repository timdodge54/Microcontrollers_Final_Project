#include "stm32l476xx.h"
#include "LCD.h"

int main(void){
	
	LCD_Init();
	LCD_DisplayString(0, (unsigned char*)"ECE 3710");
	LCD_DisplayString(1, (unsigned char*)"Utah State");
	
	while(1);
}


