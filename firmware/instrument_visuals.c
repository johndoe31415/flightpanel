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

#include "font.h"
#include "vcr-osd-mono-20.h"
#include "vcr-osd-mono-30.h"
#include "morse-font.h"
#include "instruments.h"
#include "instrument_visuals.h"
#include "frequencies.h"
#include "morse.h"

static uint32_t get_instrument_frequency_khz(const struct instrument_state_t *istate, const enum display_t display) {
	switch (display) {
		case DISPLAY_COM1:
			return com_index_to_frequency_khz(istate->com1_active_index);

		case DISPLAY_COM1_STBY:
			return com_index_to_frequency_khz(istate->com1_standby_index);

		case DISPLAY_COM2:
			return com_index_to_frequency_khz(istate->com2_active_index);

		case DISPLAY_COM2_STBY:
			return com_index_to_frequency_khz(istate->com2_standby_index);

		case DISPLAY_NAV1:
			return nav_index_to_frequency_khz(istate->nav1_active_index);

		case DISPLAY_NAV1_STBY:
			return nav_index_to_frequency_khz(istate->nav1_standby_index);

		case DISPLAY_NAV2:
			return nav_index_to_frequency_khz(istate->nav2_active_index);

		case DISPLAY_NAV2_STBY:
			return nav_index_to_frequency_khz(istate->nav2_standby_index);

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
		blit_string_to_cursor(&font_morse_font, morse, surface, &cursor, false);
		blit_string_to_cursor(&font_morse_font, " ", surface, &cursor, false);
	}
}

static void redraw_com_nav_display(const struct surface_t *surface, const struct instrument_state_t *istate, uint32_t frequency_khz, const char *ident) {
	char text[16];
	int mhz = frequency_khz / 1000;
	int khz = frequency_khz % 1000;
	sprintf(text, "%3d.%03d", mhz, khz);
	struct cursor_t cursor = { 4, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);
	draw_ident(surface, ident);
}

static void redraw_adf_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	char text[16];
	struct cursor_t cursor = { 4, 25 };
	snprintf(text, sizeof(text), "%4d kHz", 123);
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

	draw_ident(surface, "LUB");
}

static void redraw_dme_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	char text[16];
	struct cursor_t cursor = { 4, 25 };
	snprintf(text, sizeof(text), "%3d.%d nm", 123, 9);
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

	cursor = (struct cursor_t) { 4, 55 };
	snprintf(text, sizeof(text), "%3d kt", 231);
	blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
}

static void redraw_ap_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);
	if ((!istate->ap.active) || (istate->ap.hold & (HOLD_ALTITUDE | HOLD_IAS | HOLD_HEADING | HOLD_NAVIGATION | HOLD_APPROACH)) == 0) {
		struct cursor_t cursor = { 10, 45 };
		blit_string_to_cursor(&font_vcr_osd_mono_30, "AP OFF", surface, &cursor, false);
	} else {
		char text[16];
		if (istate->ap.hold & HOLD_ALTITUDE) {
			struct cursor_t cursor = { 0, 23 };
			snprintf(text, sizeof(text), "FL%d", (istate->ap.altitude + 50) / 100);
			blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);

			cursor = (struct cursor_t) { 0, 54 };
			snprintf(text, sizeof(text), "%+d", istate->ap.climbrate);
			blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
		}

		{
			struct cursor_t cursor = { 80, 54 };
			if (istate->ap.hold & HOLD_REVERSE) {
				blit_rectangle(surface, cursor.x - 1, cursor.y - 18, (istate->ap.hold & HOLD_HEADING) ? 50 : 40, 20);
			}
			if (istate->ap.hold & HOLD_HEADING) {
				snprintf(text, sizeof(text), "%3d" CHAR_DEGREES, istate->ap.heading);
				blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, istate->ap.hold & HOLD_REVERSE);
			} else if (istate->ap.hold & HOLD_NAVIGATION) {
				blit_string_to_cursor(&font_vcr_osd_mono_20, "NAV", surface, &cursor, istate->ap.hold & HOLD_REVERSE);
			} else if (istate->ap.hold & HOLD_APPROACH) {
				blit_string_to_cursor(&font_vcr_osd_mono_20, "APR", surface, &cursor, istate->ap.hold & HOLD_REVERSE);
			}
		}

		if (istate->ap.hold & HOLD_IAS) {
			struct cursor_t cursor = { 67, 23 };
			snprintf(text, sizeof(text), "%3dkt", istate->ap.ias);
			blit_string_to_cursor(&font_vcr_osd_mono_20, text, surface, &cursor, false);
		}
	}
}

static void redraw_xpdr_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	surface_clear(surface);

	char text[16];
	sprintf(text, "%04d", istate->xpdr.squawk);

	struct cursor_t cursor = { 28, 35 };
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);

	if (istate->xpdr.edit_char) {
		const int char_width = 18;
		blit_rectangle(surface, 28 + (char_width * (istate->xpdr.edit_char - 1)), 35, char_width, 2);
	}

	if (istate->xpdr.identing) {
		cursor = (struct cursor_t){ 34, 60 };
		blit_rectangle(surface, cursor.x - 1, cursor.y - 18, 63, 20);
		blit_string_to_cursor(&font_vcr_osd_mono_20, "IDENT", surface, &cursor, true);
	}
}

void redraw_display(const struct surface_t *surface, const struct instrument_state_t *istate, enum display_t display) {
	switch (display) {
		case DISPLAY_COM1:
		case DISPLAY_COM1_STBY:
		case DISPLAY_COM2:
		case DISPLAY_COM2_STBY:
		case DISPLAY_NAV1:
		case DISPLAY_NAV1_STBY:
		case DISPLAY_NAV2:
		case DISPLAY_NAV2_STBY:
			{
				uint32_t frequency_khz = get_instrument_frequency_khz(istate, display);
				const char *ident = NULL;
				if (display == DISPLAY_NAV1) {
					ident = "LUB";
				} else if (display == DISPLAY_NAV2) {
					ident = "STG";
				}
				redraw_com_nav_display(surface, istate, frequency_khz, ident);
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
