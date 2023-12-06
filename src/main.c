#include "stm32l476xx.h"

#include "usart.h"

int main(void){
	

	// keypad_main();
	buzzer_setup();
	USART_drive();
	// I2C_Initialization(I2C1);
	// uint8_t tc_controller_address = 0x91;
	// uint8_t measure_x_command = 0x12;
	//uint8_t measure_y_command = 0x13;
	char notes[7] = {'a' ,'b', 'c', 'd', 'e', 'f', 'g'};
	int i = 0;
	int timing = 0;
	
	uint8_t history = 0;
	while(1){
		//USART_Read(USART2, &history, 1);
		//USART_Write(USART2, &history, 1);
		//buzzer(notes[i]);
		buzzer();
		// uint8_t data = 0;
		//I2C_SendData(I2C1, tc_controller_address, &measure_x_command, 1);
		//I2C_RecieveData(I2C1, 0x91, &data, 1);
		if (timing % 100 == 0){
			i += 1;
		}
		if (i > 6){
			i = 0;
		}
		timing += 1;
		history = 0;
			
	}
}


