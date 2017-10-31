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
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "font.h"
#include "vcr-osd-mono-20.h"
#include "inconsolata-30.h"
#include "symbol-font.h"
#include "instruments.h"
#include "instrument_visuals.h"
#include "frequencies.h"
#include "morse.h"

static uint32_t get_instrument_frequency_khz(const struct instrument_state_t *istate, const enum display_t display) {
	switch (display) {
		case DISPLAY_COM1:
			return frequency_index_to_khz(istate->external.com_divisions, istate->external.com1.freq.active_index);

		case DISPLAY_COM1_STBY:
			return frequency_index_to_khz(istate->external.com_divisions, istate->external.com1.freq.standby_index);

		case DISPLAY_COM2:
			return frequency_index_to_khz(istate->external.com_divisions, istate->external.com2.freq.active_index);

		case DISPLAY_COM2_STBY:
			return frequency_index_to_khz(istate->external.com_divisions, istate->external.com2.freq.standby_index);

		case DISPLAY_NAV1:
			return frequency_index_to_khz(istate->external.nav_divisions, istate->external.nav1.freq.active_index);

		case DISPLAY_NAV1_STBY:
			return frequency_index_to_khz(istate->external.nav_divisions, istate->external.nav1.freq.standby_index);

		case DISPLAY_NAV2:
			return frequency_index_to_khz(istate->external.nav_divisions, istate->external.nav2.freq.active_index);

		case DISPLAY_NAV2_STBY:
			return frequency_index_to_khz(istate->external.nav_divisions, istate->external.nav2.freq.standby_index);

		default:
			return 0;
	}
}

static void draw_ident(const struct surface_t *surface, const char *ident) {
	if (!ident) {
		return;
	}

	struct cursor_t cursor = { 2, 60 };
	blit_string_to_cursor(&font_vcr_osd_mono_20, ident, surface, &cursor, false);

	cursor = (struct cursor_t) { 55, 53 };
	for (int i = 0; i < 4; i++) {
		char ident_char = ident[i];
		if (!ident_char) {
			break;
		}
		if (i == 2) {
			cursor = (struct cursor_t) { 55, 64 };
		}
		const char *morse = morse_get_code(ident_char);
		blit_string_to_cursor(&font_symbol_font, morse, surface, &cursor, false);
		blit_string_to_cursor(&font_symbol_font, " ", surface, &cursor, false);
	}
}

static void redraw_com_nav_display(const struct surface_t *surface, const struct instrument_state_t *istate, uint32_t frequency_khz, const char *ident, const char *tx_dme, bool active_obs, const uint16_t *obs) {
	char text[16];
	int mhz = frequency_khz / 1000;
	int khz = frequency_khz % 1000;
	sprintf(text, "%3d.%03d", mhz, khz);
	struct cursor_t cursor = { 4, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_inconsolata_30, text, surface, &cursor, false);
	draw_ident(surface, ident);
	if (tx_dme) {
		cursor = (struct cursor_t) { 5, 60 };
		blit_string_to_cursor(&font_vcr_osd_mono_20, tx_dme, surface, &cursor, false);
	}
	if (obs) {
		cursor = (struct cursor_t) { 70, 60 };
		sprintf(text, "%s%3d" CHAR_DEGREES, active_obs ? ">" : " ", *obs);
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
	}
}

#define RDIV(p, q)		(((p) + ((q) / 2)) / (q))		/* With rounding */
static uint32_t mbar_to_inhg_hundreds(uint32_t mbar) {
	/* inhg = mbar * 15000000 / 5079583
	 *
	 * Approximation to stay in 32 bit range (for up to 4867 mBar at least):
	 * inhg = mbar * 882353 / 298799
	 *
	 */
	return RDIV(mbar * 882353, 298799);
}

static void redraw_qnh_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);
	char text[16];
	sprintf(text, "%d", istate->external.qnh);
	struct cursor_t cursor = { TEXT_CENTER, 28 };
	blit_string_to_cursor(&font_inconsolata_30, text, surface, &cursor, false);

	cursor = (struct cursor_t) { 10, 55 };
	const uint32_t inhg = mbar_to_inhg_hundreds(istate->external.qnh);
	sprintf(text, "QNH %" PRIu32 ".%02" PRIu32, inhg / 100, inhg % 100);
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
}

static void redraw_adf_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	char text[16];
	struct cursor_t cursor = { TEXT_CENTER, 25 };
	snprintf(text, sizeof(text), "%d kHz", istate->external.adf.frequency_khz);
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

	draw_ident(surface, istate->internal.ident.adf);
}

static void redraw_dme_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	if (istate->internal.dme.available) {
		char text[16];
		struct cursor_t cursor = { TEXT_CENTER, 25 };
		snprintf(text, sizeof(text), "%d.%d nm", istate->internal.dme.distance_tenth_nm / 10, istate->internal.dme.distance_tenth_nm % 10);
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

		cursor = (struct cursor_t) { TEXT_CENTER, 55 };
		snprintf(text, sizeof(text), "%d kt", istate->internal.dme.velocity);
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
	} else {
		struct cursor_t cursor = { 28, 40 };
		blit_string_to_cursor(&font_vcr_osd_mono_20, "No DME", surface, &cursor, false);
	}
}

static void redraw_ap_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);
	char text[16];
	{
		struct cursor_t cursor = { 0, 23 };
		snprintf(text, sizeof(text), "FL%d", (istate->external.ap.altitude + 50) / 100);
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

		cursor = (struct cursor_t) { 0, 54 };
		if (istate->external.ap.climbrate != 0) {
			snprintf(text, sizeof(text), "%+d", istate->external.ap.climbrate / 100);
		} else {
			snprintf(text, sizeof(text), "0");
		}
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
	}

	{
		struct cursor_t cursor = { 41, 54 };
		if (istate->external.ap.state & AP_STATE_BACKCOURSE) {
			blit_rectangle(surface, cursor.x - 1 + (istate->external.ap.state & AP_GLIDESLOPE_HOLD ? 0 : 35), cursor.y - 18, 128, 20);
		}
		if (istate->external.ap.state & AP_GLIDESLOPE_ARMED) {
			blit_string_to_cursor(&font_vcr_osd_mono_20, "G-", surface, &cursor, istate->external.ap.state & AP_STATE_BACKCOURSE);
		} else if (istate->external.ap.state & AP_GLIDESLOPE_HOLD) {
			blit_string_to_cursor(&font_vcr_osd_mono_20, "G+", surface, &cursor, istate->external.ap.state & AP_STATE_BACKCOURSE);
		}

		cursor = (struct cursor_t) { TEXT_RIGHT_JUSTIFY, 54 };
		if (istate->external.ap.state & AP_NAVIGATION_ARMED) {
			blit_string_to_cursor(&font_vcr_osd_mono_20, "L-", surface, &cursor, istate->external.ap.state & AP_STATE_BACKCOURSE);
		} else if (istate->external.ap.state & AP_NAVIGATION_HOLD) {
			blit_string_to_cursor(&font_vcr_osd_mono_20, "L+", surface, &cursor, istate->external.ap.state & AP_STATE_BACKCOURSE);
		} else {
			snprintf(text, sizeof(text), "%3d" CHAR_DEGREES, istate->external.ap.heading);
			blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, istate->external.ap.state & AP_STATE_BACKCOURSE);
		}
	}

	{
		struct cursor_t cursor = { 67, 23 };
		snprintf(text, sizeof(text), "%3dkt", istate->external.ap.ias);
		blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
	}
}

static void redraw_xpdr_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	char text[16];
	sprintf(text, "%04d", istate->external.xpdr.squawk);

	struct cursor_t cursor = { 28, 35 };
	blit_string_to_cursor(&font_inconsolata_30, text, surface, &cursor, false);

	if (istate->internal.xpdr.edit_char) {
		const int char_width = 18;
		blit_rectangle(surface, 28 + (char_width * (istate->internal.xpdr.edit_char - 1)), 37, char_width, 2);
	}

	if (istate->external.xpdr.state & XPDR_MODE_IDENTING) {
		cursor = (struct cursor_t){ 34, 60 };
		blit_rectangle(surface, cursor.x - 1, cursor.y - 18, 63, 20);
		blit_string_to_cursor(&font_vcr_osd_mono_20, "IDENT", surface, &cursor, true);
	}
}

void redraw_display(const struct surface_t *surface, const struct instrument_state_t *istate, enum display_t display) {
	if ((istate->external.flip_switches & SWITCH_MASTER) == 0) {
		/* All displays off, MASTER off */
		surface_clear(surface);
		return;
	}

	switch (display) {
		case DISPLAY_COM1:
		case DISPLAY_COM1_STBY:
		case DISPLAY_COM2:
		case DISPLAY_COM2_STBY:
		case DISPLAY_NAV1:
		case DISPLAY_NAV1_STBY:
		case DISPLAY_NAV2:
		case DISPLAY_NAV2_STBY:
			if ((display != DISPLAY_QNH) || (istate->internal.screen_mplex.qnh == DEFAULT)) {
				uint32_t frequency_khz = get_instrument_frequency_khz(istate, display);
				const char *ident = NULL;
				const char *tx_dme = NULL;
				const uint16_t *obs = NULL;
				bool active_obs = false;
				if (display == DISPLAY_NAV1) {
					if (!istate->internal.ident.nav1_ident_inhibit_timeout) {
						ident = istate->internal.ident.nav1;
					}
				} else if (display == DISPLAY_NAV2) {
					if (!istate->internal.ident.nav2_ident_inhibit_timeout) {
						ident = istate->internal.ident.nav2;
					}
				} else if (display == DISPLAY_COM1) {
					if (istate->external.tx_radio_id == 1) {
						tx_dme = "TX";
					}
				} else if (display == DISPLAY_COM2) {
					if (istate->external.tx_radio_id == 2) {
						tx_dme = "TX";
					}
				} else if (display == DISPLAY_NAV1_STBY) {
					obs = &istate->external.nav1.obs;
					active_obs = istate->internal.active_obs == 0;
				} else if (display == DISPLAY_NAV2_STBY) {
					obs = &istate->external.nav2.obs;
					active_obs = istate->internal.active_obs == 1;
				}
				redraw_com_nav_display(surface, istate, frequency_khz, ident, tx_dme, active_obs, obs);
			} else {
				redraw_qnh_display(surface, istate);
			}
			break;

		case DISPLAY_ADF:
			redraw_adf_display(surface, istate);
			break;

		case DISPLAY_DME:
			redraw_dme_display(surface, istate);
			break;

		case DISPLAY_AP:
			redraw_ap_display(surface, istate);
			break;

		case DISPLAY_XPDR:
			redraw_xpdr_display(surface, istate);
			break;
	}
}
