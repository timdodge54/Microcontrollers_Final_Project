#include "stm32l476xx.h"
#include "keypad_init.h"

int main(void){
	

	keypad_main();
	buzzer_setup();
	I2C_Initialization(I2C1);
	uint8_t tc_controller_address = 0x91;
	uint8_t measure_x_command = 0x12;
	uint8_t measure_y_command = 0x13;
	while(1){
		//buzzer();
		uint8_t data = 0;
		I2C_SendData(I2C1, tc_controller_address, &measure_x_command, 1);
		I2C_RecieveData(I2C1, 0x91, &data, 1);
		for (int i = 0; i < 50000; ++i);
		
	}
}


