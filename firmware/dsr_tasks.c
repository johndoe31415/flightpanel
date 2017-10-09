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

#include "dsr_tasks.h"
#include "displays.h"
#include "instruments.h"
#include "iomux.h"
#include "debugconsole.h"

static struct dsr_runtime_info_t dsr_runtime_info[NUMBER_OF_DSRS];
static const struct dsr_task_definition_t dsr_definitions[NUMBER_OF_DSRS] = {
	[DSR_TASK_IDLE] = {
		.callback = dsr_idle_task,
	},
	[DSR_TASK_IOMPLEX_FINISHED] = {
		.callback = dsr_iomux_dma_finished,
	},
	[DSR_TASK_DISPLAY_UPDATE_FINISHED] = {
		.callback = dsr_display_dma_finished,
	},
	[DSR_TASK_EXECUTE_DEBUG_COMMAND] = {
		.callback = dsr_execute_debug_command,
	},
};

bool dsr_is_pending(enum dsr_task_t task) {
	return dsr_runtime_info[task].pending;
}

void dsr_mark_pending_delayed(enum dsr_task_t task, uint32_t delay) {
	dsr_runtime_info[task].pending = true;
	dsr_runtime_info[task].call_delay = delay;
}

void dsr_mark_pending(enum dsr_task_t task) {
	dsr_mark_pending_delayed(task, 0);
}


void execute_dsrs(void) {
	for (int i = 0; i < NUMBER_OF_DSRS; i++) {
		if (dsr_runtime_info[i].pending) {
			if (dsr_runtime_info[i].call_delay == 0) {
				dsr_runtime_info[i].pending = false;
				dsr_definitions[i].callback();
				break;
			} else {
				dsr_runtime_info[i].call_delay--;
			}
		}
	}
}

void execute_dsr_loop(void) {
	while (true) {
		execute_dsrs();
	}
}

