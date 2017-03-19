#include <stm32f4xx_tim.h>
#include "timer.h"

void HAL_IncTick(void);

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void SysTick_Handler(void) {
	HAL_IncTick();
}
