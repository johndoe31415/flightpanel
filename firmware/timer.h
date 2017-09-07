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

#ifndef __TIMER_H__
#define __TIMER_H__

/* These values are only valid for 168 MHz operation */
#define LOOPCOUNT_35NS			0
#define LOOPCOUNT_50NS			1
#define LOOPCOUNT_80NS			2
#define LOOPCOUNT_110NS			3
#define LOOPCOUNT_1MS			42000

/* Asymptotic behavior of this delay loop: 4 clock cycles per iteration count.
 * Therefore:
 *     t_delay = 4 / f_cpu * itercnt
 *     itercnt = t_delay * f_cpu / 4
 *     f_cpu = itercnt / t_delay * 4
 *
 * Example: f_cpu = 168 MHz, itercnt = 10000, t_delay = 238 µs
 * Example: f_cpu = 168 MHz, t_delay = 1 ms, itercnt = 42000
 */

#define delay_loopcnt(nopcnt) { int _nopcnt = nopcnt; \
	__asm__ __volatile__(		\
		"0:"						"\n\t"				\
		"cbz %0, 1f"				"\n\t"				\
		"subs %0, #1"				"\n\t"				\
		"b 0b"						"\n\t"				\
		"1:"						"\n\t"				\
		: "+r"(_nopcnt)									\
	);													\
	}

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
void delay_millis(uint32_t millisecs);
void TIM3_IRQHandler(void);
void SysTick_Handler(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
