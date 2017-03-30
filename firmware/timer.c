#include <stm32f4xx_tim.h>
#include "timer.h"
#include "instruments.h"
#include "pinmap.h"

void HAL_IncTick(void);

static volatile uint32_t milli_ticks;

void delay_millis(uint32_t millisecs) {
	milli_ticks = millisecs;
	while (milli_ticks);
}


void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void SysTick_Handler(void) {
	Dbg4_SetHIGH();
	hid_tick();

	if (milli_ticks) {
		milli_ticks--;
	}
	HAL_IncTick();
	Dbg4_SetLOW();
}
