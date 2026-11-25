/*
 * rf.cpp
 *
 *
 */

#include "main.h"
#include "rf.h"
#include "timing.h"



void transmit(uint32_t highPulses, uint32_t lowPulses){
	HAL_GPIO_WritePin(RF_SEND_GPIO_Port, RF_SEND_Pin, GPIO_PIN_RESET);
	delay350Microseconds(highPulses);

	HAL_GPIO_WritePin(RF_SEND_GPIO_Port, RF_SEND_Pin, GPIO_PIN_SET);
	delay350Microseconds(lowPulses);
}


void sendSequence(uint16_t bitSequence){

	for(int i = 0; i<10 ; i++){

		for(int j = 0 ; j < 12 ; j++){

			transmit(1, 3);

			uint8_t currentBit = (bitSequence >> (11 - j)) & 1U;

			if(currentBit == 1) {
				transmit(1, 3);
			}
			else{
				transmit(3, 1);
			}

		}

		transmit(1,31);


	}

}
