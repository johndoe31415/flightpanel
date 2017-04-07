#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include "emulator.hpp"

void EmulatedConnection::event_loop() {
	_loop_running = true;
	while (_loop_running) {
		fsconnection_incoming_data(&_instr);
		sleep(1);
	}
}

static int rand_value(int maxvalue) {
	return rand() % maxvalue;
}

void EmulatedConnection::randomize(struct vhf_data_t &vhf) {
	vhf.freq_active_khz = 118000 + (25 * rand_value(19 * 40));
	vhf.freq_standby_khz = 118000 + (25 * rand_value(19 * 40));
	vhf.rx = true;
	vhf.tx = false;
}

void EmulatedConnection::randomize(struct nav_data_t &nav) {
	nav.freq_active_khz = 108000 + (50 * rand_value(10 * 20));
	nav.freq_standby_khz = 108000 + (50 * rand_value(10 * 20));
	nav.obs = rand_value(360);
	nav.sound = rand_value(2);
}

EmulatedConnection::EmulatedConnection() {
	_loop_running = false;
	memset(&_instr, 0, sizeof(_instr));
	randomize(_instr.vhf1);
	randomize(_instr.vhf2);
	randomize(_instr.nav1);
	randomize(_instr.nav2);
}

EmulatedConnection::~EmulatedConnection() {
}

