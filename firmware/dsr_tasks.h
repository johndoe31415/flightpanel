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

#ifndef __DSR_TASKS_H__
#define __DSR_TASKS_H__

#include <stdint.h>
#include <stdbool.h>

typedef void (*dsr_task_callback_t)(void);

struct dsr_runtime_info_t {
	bool pending;
};
struct dsr_task_definition_t {
	dsr_task_callback_t callback;
};

enum dsr_task_t {
	DSR_TASK_IOMPLEX_FINISHED,
	DSR_TASK_DISPLAY_UPDATE_FINISHED,
	DSR_TASK_EXECUTE_DEBUG_COMMAND,
	DSR_TASK_IDLE,
	NUMBER_OF_DSRS,
};

/*************** AUTO GENERATED SECTION FOLLOWS ***************/
bool dsr_is_pending(enum dsr_task_t task);
void dsr_mark_pending(enum dsr_task_t task);
void execute_dsrs(void);
void execute_dsr_loop(void);
/***************  AUTO GENERATED SECTION ENDS   ***************/

#endif
