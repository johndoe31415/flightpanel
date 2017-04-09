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
	memset(&nav, 0, sizeof(nav));
	nav.freq_active_khz = 108000 + (50 * rand_value(10 * 20));
	nav.freq_standby_khz = 108000 + (50 * rand_value(10 * 20));
	nav.obs = rand_value(360);
	nav.sound = rand_value(2);
	if (!rand_value(4)) {
		for (int i = 0; i < 3; i++) {
			nav.ident[i] = 'A' + rand_value(26);
		}
	}
}

void EmulatedConnection::randomize(struct adf_data_t &adf) {
	adf.freq_hz = 100000 + (100 * rand_value(8000));
	if (!rand_value(4)) {
		for (int i = 0; i < 3; i++) {
			adf.ident[i] = 'A' + rand_value(26);
		}
	}
	adf.compass_rose = rand_value(360);
}

void EmulatedConnection::randomize(struct dme_data_t &dme) {
	dme.available = rand_value(2);
	dme.nav_id = rand_value(2) + 1;
	if (dme.available) {
		dme.distance_nm_tenths = rand_value(700);
		dme.speed_kt = rand_value(120);
	}
}

void EmulatedConnection::randomize(struct light_data_t &lights) {
	lights.beacon = rand_value(2);
	lights.landing = rand_value(2);
	lights.taxi = rand_value(2);
	lights.nav = rand_value(2);
	lights.strobe = rand_value(2);
}

void EmulatedConnection::randomize(struct ap_data_t &ap) {
	memset(&ap, 0, sizeof(ap));
	ap.active = rand_value(2);
	ap.climbrate_ft_per_min = (rand_value(4) * 100) + 400;
	ap.heading = rand_value(360);
	if (ap.active) {
		int mode = rand_value(3);
		if (mode == 0) {
			ap.hdg_hold = true;
		} else if (mode == 1) {
			ap.nav_hold = true;
		} else if (mode == 2) {
			ap.apr_hold = true;
			ap.rev_hold = rand_value(2);
		}
		ap.ias_hold = rand_value(2);
		ap.alt_hold = rand_value(2);
	}
	ap.altitude_ft = 1200 + 100 * rand_value(60);
}

void EmulatedConnection::randomize(struct transponder_data_t &xpdr) {
	xpdr.squawk = 0;
	for (int i = 0; i < 4; i++) {
		xpdr.squawk = (10 * xpdr.squawk) + rand_value(8);
	}
}

void EmulatedConnection::randomize(struct misc_data_t &misc) {
	misc.ias_kt = 60 + rand_value(70);
	misc.indicated_alt_ft = 1200 + rand_value(7000);
	misc.qnh_millibar = 970 + rand_value(60);
	misc.guide_gps = rand_value(2);
}

EmulatedConnection::EmulatedConnection() {
	_loop_running = false;
	srand(time(NULL));
	memset(&_instr, 0, sizeof(_instr));
	randomize(_instr.vhf1);
	randomize(_instr.vhf2);
	if (rand_value(2)) {
		_instr.vhf1.tx = true;
	} else {
		_instr.vhf1.tx = true;
	}
	randomize(_instr.nav1);
	randomize(_instr.nav2);
	randomize(_instr.adf);
	randomize(_instr.dme);
	randomize(_instr.lights);
	randomize(_instr.ap);
	randomize(_instr.xpdr);
	randomize(_instr.misc);
}

EmulatedConnection::~EmulatedConnection() {
}

