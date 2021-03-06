/**
 *	flightpanel - A Cortex-M4 based USB flight panel for flight simulators.
 *	Copyright (C) 2017-2017 Johannes Bauer
 *
 *	This file is part of flightpanel.
 *
 *	flightpanel is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; this program is ONLY licensed under
 *	version 3 of the License, later versions are explicitly excluded.
 *
 *	flightpanel is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with flightpanel; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	Johannes Bauer <JohannesBauer@gmx.de>
**/

#include <stdio.h>
#include <stdbool.h>
#include <stm32f4xx_tim.h>
#include "timer.h"
#include "instruments.h"
#include "iomux.h"
#include "debugconsole.h"
#include "pinmap.h"
#include "displays.h"
#include "dsr_tasks.h"

void HAL_IncTick(void);

static volatile uint32_t milli_ticks;

void delay_millis(uint32_t millisecs) {
	milli_ticks = millisecs;
	while (milli_ticks);
}

static void timeout_callback(void) {
}

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		TIM_Cmd(TIM3, DISABLE);
		timeout_callback();
	}
}

void timeout_timer_enable(void) {
	TIM_SetCounter(TIM3, 0);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_Cmd(TIM3, ENABLE);
}

void timeout_timer_disable(void) {
	TIM_Cmd(TIM3, DISABLE);
}

void SysTick_Handler(void) {
	if (milli_ticks) {
		milli_ticks--;
	}
	HAL_IncTick();
	iomux_trigger();
	hid_tick();
	debugconsole_tick();
	dsr_mark_pending(DSR_TASK_IDLE);
}
