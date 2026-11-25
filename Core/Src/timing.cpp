

#include "main.h"
#include "timing.h"


extern "C" {
    extern TIM_HandleTypeDef htim2;
}

void delay350Microseconds(uint32_t n) {

	uint32_t timeToWait = n * 350U; // Timer ticks (1 tick = 1 us) , TIM2 -> 1 MHz

	__HAL_TIM_SET_COUNTER(&htim2, 0);	// resetting the timer

	while(1) {
		if(__HAL_TIM_GET_COUNTER(&htim2) >= timeToWait){
			break;
		}
	}


}
