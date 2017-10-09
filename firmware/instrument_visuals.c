
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
#include "instruments.h"
#include "instrument_visuals.h"
#include "frequencies.h"

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

static void redraw_com_nav_display(const struct surface_t *surface, const struct instrument_state_t *istate, uint32_t frequency_khz) {
	char text[16];
	int mhz = frequency_khz / 1000;
	int khz = frequency_khz % 1000;
	sprintf(text, "%3d.%03d", mhz, khz);
	struct cursor_t cursor = { 4, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);
}

static void redraw_adf_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	char text[16];
	sprintf(text, "ADF");
	struct cursor_t cursor = { 0, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);
}

static void redraw_dme_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	char text[16];
	sprintf(text, "DME");
	struct cursor_t cursor = { 0, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);
}

static void redraw_ap_display(const struct surface_t *surface, const struct instrument_state_t *istate) {
	char text[16];
	sprintf(text, "AP");
	struct cursor_t cursor = { 0, 35 };
	surface_clear(surface);
	blit_string_to_cursor(&font_vcr_osd_mono_30, text, surface, &cursor, false);
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
				redraw_com_nav_display(surface, istate, frequency_khz);
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
