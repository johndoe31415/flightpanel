#include <stdio.h>
#include <string.h>
#include <math.h>
#include "xplane-datadefs.hpp"

void xplane_resolve_refs(struct xplane_datadef_refs_t &datadefs) {
	datadefs.sim_cockpit_radios_adf1_freq_hz = XPLMFindDataRef("sim/cockpit/radios/adf1_freq_hz");
	if (!datadefs.sim_cockpit_radios_adf1_freq_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_freq_hz\n");
	}
	datadefs.sim_cockpit_radios_adf2_freq_hz = XPLMFindDataRef("sim/cockpit/radios/adf2_freq_hz");
	if (!datadefs.sim_cockpit_radios_adf2_freq_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_freq_hz\n");
	}
	datadefs.sim_cockpit_radios_adf1_stdby_freq_hz = XPLMFindDataRef("sim/cockpit/radios/adf1_stdby_freq_hz");
	if (!datadefs.sim_cockpit_radios_adf1_stdby_freq_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_stdby_freq_hz\n");
	}
	datadefs.sim_cockpit_radios_adf2_stdby_freq_hz = XPLMFindDataRef("sim/cockpit/radios/adf2_stdby_freq_hz");
	if (!datadefs.sim_cockpit_radios_adf2_stdby_freq_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_stdby_freq_hz\n");
	}
	datadefs.sim_cockpit_radios_adf1_dir_degt = XPLMFindDataRef("sim/cockpit/radios/adf1_dir_degt");
	if (!datadefs.sim_cockpit_radios_adf1_dir_degt) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_dir_degt\n");
	}
	datadefs.sim_cockpit_radios_adf2_dir_degt = XPLMFindDataRef("sim/cockpit/radios/adf2_dir_degt");
	if (!datadefs.sim_cockpit_radios_adf2_dir_degt) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_dir_degt\n");
	}
	datadefs.sim_cockpit_radios_adf1_dme_dist_m = XPLMFindDataRef("sim/cockpit/radios/adf1_dme_dist_m");
	if (!datadefs.sim_cockpit_radios_adf1_dme_dist_m) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_dme_dist_m\n");
	}
	datadefs.sim_cockpit_radios_adf2_dme_dist_m = XPLMFindDataRef("sim/cockpit/radios/adf2_dme_dist_m");
	if (!datadefs.sim_cockpit_radios_adf2_dme_dist_m) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_dme_dist_m\n");
	}
	datadefs.sim_cockpit_radios_adf1_dme_speed_kts = XPLMFindDataRef("sim/cockpit/radios/adf1_dme_speed_kts");
	if (!datadefs.sim_cockpit_radios_adf1_dme_speed_kts) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_dme_speed_kts\n");
	}
	datadefs.sim_cockpit_radios_adf2_dme_speed_kts = XPLMFindDataRef("sim/cockpit/radios/adf2_dme_speed_kts");
	if (!datadefs.sim_cockpit_radios_adf2_dme_speed_kts) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_dme_speed_kts\n");
	}
	datadefs.sim_cockpit_radios_adf1_dme_time_secs = XPLMFindDataRef("sim/cockpit/radios/adf1_dme_time_secs");
	if (!datadefs.sim_cockpit_radios_adf1_dme_time_secs) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_dme_time_secs\n");
	}
	datadefs.sim_cockpit_radios_adf2_dme_time_secs = XPLMFindDataRef("sim/cockpit/radios/adf2_dme_time_secs");
	if (!datadefs.sim_cockpit_radios_adf2_dme_time_secs) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_dme_time_secs\n");
	}
	datadefs.sim_cockpit_radios_adf1_cardinal_dir = XPLMFindDataRef("sim/cockpit/radios/adf1_cardinal_dir");
	if (!datadefs.sim_cockpit_radios_adf1_cardinal_dir) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_cardinal_dir\n");
	}
	datadefs.sim_cockpit_radios_adf1_cardinal_dir2 = XPLMFindDataRef("sim/cockpit/radios/adf1_cardinal_dir2");
	if (!datadefs.sim_cockpit_radios_adf1_cardinal_dir2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_cardinal_dir2\n");
	}
	datadefs.sim_cockpit_radios_adf2_cardinal_dir = XPLMFindDataRef("sim/cockpit/radios/adf2_cardinal_dir");
	if (!datadefs.sim_cockpit_radios_adf2_cardinal_dir) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_cardinal_dir\n");
	}
	datadefs.sim_cockpit_radios_adf2_cardinal_dir2 = XPLMFindDataRef("sim/cockpit/radios/adf2_cardinal_dir2");
	if (!datadefs.sim_cockpit_radios_adf2_cardinal_dir2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_cardinal_dir2\n");
	}
	datadefs.sim_cockpit_radios_adf1_has_dme = XPLMFindDataRef("sim/cockpit/radios/adf1_has_dme");
	if (!datadefs.sim_cockpit_radios_adf1_has_dme) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf1_has_dme\n");
	}
	datadefs.sim_cockpit_radios_adf2_has_dme = XPLMFindDataRef("sim/cockpit/radios/adf2_has_dme");
	if (!datadefs.sim_cockpit_radios_adf2_has_dme) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/adf2_has_dme\n");
	}
	datadefs.sim_cockpit_radios_dme5_has_dme = XPLMFindDataRef("sim/cockpit/radios/dme5_has_dme");
	if (!datadefs.sim_cockpit_radios_dme5_has_dme) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/dme5_has_dme\n");
	}
	datadefs.sim_cockpit_radios_nav_com_adf_mode = XPLMFindDataRef("sim/cockpit/radios/nav_com_adf_mode");
	if (!datadefs.sim_cockpit_radios_nav_com_adf_mode) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/radios/nav_com_adf_mode\n");
	}
	datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector = XPLMFindDataRef("sim/cockpit/switches/RMI_l_vor_adf_selector");
	if (!datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/switches/RMI_l_vor_adf_selector\n");
	}
	datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector2 = XPLMFindDataRef("sim/cockpit/switches/RMI_l_vor_adf_selector2");
	if (!datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/switches/RMI_l_vor_adf_selector2\n");
	}
	datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector = XPLMFindDataRef("sim/cockpit/switches/RMI_r_vor_adf_selector");
	if (!datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/switches/RMI_r_vor_adf_selector\n");
	}
	datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector2 = XPLMFindDataRef("sim/cockpit/switches/RMI_r_vor_adf_selector2");
	if (!datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit/switches/RMI_r_vor_adf_selector2\n");
	}
	datadefs.sim_operation_failures_rel_adf1 = XPLMFindDataRef("sim/operation/failures/rel_adf1");
	if (!datadefs.sim_operation_failures_rel_adf1) {
		fprintf(stderr, "Unresolvable symbol: sim/operation/failures/rel_adf1\n");
	}
	datadefs.sim_operation_failures_rel_adf2 = XPLMFindDataRef("sim/operation/failures/rel_adf2");
	if (!datadefs.sim_operation_failures_rel_adf2) {
		fprintf(stderr, "Unresolvable symbol: sim/operation/failures/rel_adf2\n");
	}
	datadefs.sim_operation_override_override_adf = XPLMFindDataRef("sim/operation/override/override_adf");
	if (!datadefs.sim_operation_override_override_adf) {
		fprintf(stderr, "Unresolvable symbol: sim/operation/override/override_adf\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_power = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_power");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_power) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_power\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_power = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_power");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_power) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_power\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_standby_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_standby_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_standby_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_standby_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_pilot");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_pilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_pilot");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_pilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_copilot");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_copilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_copilot");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_copilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/RMI_left_use_adf_pilot");
	if (!datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/RMI_left_use_adf_pilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/RMI_left_use_adf_copilot");
	if (!datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/RMI_left_use_adf_copilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/RMI_right_use_adf_pilot");
	if (!datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/RMI_right_use_adf_pilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot = XPLMFindDataRef("sim/cockpit2/radios/actuators/RMI_right_use_adf_copilot");
	if (!datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/RMI_right_use_adf_copilot\n");
	}
	datadefs.sim_cockpit2_radios_actuators_nav_com_adf_mode = XPLMFindDataRef("sim/cockpit2/radios/actuators/nav_com_adf_mode");
	if (!datadefs.sim_cockpit2_radios_actuators_nav_com_adf_mode) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/nav_com_adf_mode\n");
	}
	datadefs.sim_cockpit2_radios_actuators_audio_nav_selection = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_nav_selection");
	if (!datadefs.sim_cockpit2_radios_actuators_audio_nav_selection) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/audio_nav_selection\n");
	}
	datadefs.sim_cockpit2_radios_actuators_audio_selection_adf1 = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_adf1");
	if (!datadefs.sim_cockpit2_radios_actuators_audio_selection_adf1) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/audio_selection_adf1\n");
	}
	datadefs.sim_cockpit2_radios_actuators_audio_selection_adf2 = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_selection_adf2");
	if (!datadefs.sim_cockpit2_radios_actuators_audio_selection_adf2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/audio_selection_adf2\n");
	}
	datadefs.sim_cockpit2_radios_actuators_audio_volume_adf1 = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_volume_adf1");
	if (!datadefs.sim_cockpit2_radios_actuators_audio_volume_adf1) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/audio_volume_adf1\n");
	}
	datadefs.sim_cockpit2_radios_actuators_audio_volume_adf2 = XPLMFindDataRef("sim/cockpit2/radios/actuators/audio_volume_adf2");
	if (!datadefs.sim_cockpit2_radios_actuators_audio_volume_adf2) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/audio_volume_adf2\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_left_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_left_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_left_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_left_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_left_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_left_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_left_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_left_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_right_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_right_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_right_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_right_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_right_frequency_hz = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_right_frequency_hz");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_right_frequency_hz) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_right_frequency_hz\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf1_right_is_selected = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf1_right_is_selected");
	if (!datadefs.sim_cockpit2_radios_actuators_adf1_right_is_selected) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf1_right_is_selected\n");
	}
	datadefs.sim_cockpit2_radios_actuators_adf2_right_is_selected = XPLMFindDataRef("sim/cockpit2/radios/actuators/adf2_right_is_selected");
	if (!datadefs.sim_cockpit2_radios_actuators_adf2_right_is_selected) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/actuators/adf2_right_is_selected\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_bearing_deg_mag");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_bearing_deg_mag\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_bearing_deg_mag");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_bearing_deg_mag\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_relative_bearing_deg");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_relative_bearing_deg\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_relative_bearing_deg");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_relative_bearing_deg\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_has_dme = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_has_dme");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_has_dme) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_has_dme\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_has_dme = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_has_dme");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_has_dme) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_has_dme\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_dme_distance_nm = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_dme_distance_nm");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_dme_distance_nm) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_dme_distance_nm\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_dme_distance_nm = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_dme_distance_nm");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_dme_distance_nm) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_dme_distance_nm\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_dme_speed_kts = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_dme_speed_kts");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_dme_speed_kts) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_dme_speed_kts\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_dme_speed_kts = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_dme_speed_kts");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_dme_speed_kts) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_dme_speed_kts\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_dme_time_min = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_dme_time_min");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_dme_time_min) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_dme_time_min\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_dme_time_min = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_dme_time_min");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_dme_time_min) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_dme_time_min\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf1_nav_id = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf1_nav_id");
	if (!datadefs.sim_cockpit2_radios_indicators_adf1_nav_id) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf1_nav_id\n");
	}
	datadefs.sim_cockpit2_radios_indicators_adf2_nav_id = XPLMFindDataRef("sim/cockpit2/radios/indicators/adf2_nav_id");
	if (!datadefs.sim_cockpit2_radios_indicators_adf2_nav_id) {
		fprintf(stderr, "Unresolvable symbol: sim/cockpit2/radios/indicators/adf2_nav_id\n");
	}
}

void xplane_get_data(const struct xplane_datadef_refs_t &datadefs, struct xplane_datadef_data_t &data) {
	data.sim_cockpit_radios_adf1_freq_hz = XPLMGetDatai(datadefs.sim_cockpit_radios_adf1_freq_hz);
	data.sim_cockpit_radios_adf2_freq_hz = XPLMGetDatai(datadefs.sim_cockpit_radios_adf2_freq_hz);
	data.sim_cockpit_radios_adf1_stdby_freq_hz = XPLMGetDatai(datadefs.sim_cockpit_radios_adf1_stdby_freq_hz);
	data.sim_cockpit_radios_adf2_stdby_freq_hz = XPLMGetDatai(datadefs.sim_cockpit_radios_adf2_stdby_freq_hz);
	data.sim_cockpit_radios_adf1_dir_degt = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_dir_degt);
	data.sim_cockpit_radios_adf2_dir_degt = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_dir_degt);
	data.sim_cockpit_radios_adf1_dme_dist_m = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_dme_dist_m);
	data.sim_cockpit_radios_adf2_dme_dist_m = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_dme_dist_m);
	data.sim_cockpit_radios_adf1_dme_speed_kts = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_dme_speed_kts);
	data.sim_cockpit_radios_adf2_dme_speed_kts = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_dme_speed_kts);
	data.sim_cockpit_radios_adf1_dme_time_secs = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_dme_time_secs);
	data.sim_cockpit_radios_adf2_dme_time_secs = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_dme_time_secs);
	data.sim_cockpit_radios_adf1_cardinal_dir = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_cardinal_dir);
	data.sim_cockpit_radios_adf1_cardinal_dir2 = XPLMGetDataf(datadefs.sim_cockpit_radios_adf1_cardinal_dir2);
	data.sim_cockpit_radios_adf2_cardinal_dir = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_cardinal_dir);
	data.sim_cockpit_radios_adf2_cardinal_dir2 = XPLMGetDataf(datadefs.sim_cockpit_radios_adf2_cardinal_dir2);
	data.sim_cockpit_radios_adf1_has_dme = XPLMGetDatai(datadefs.sim_cockpit_radios_adf1_has_dme);
	data.sim_cockpit_radios_adf2_has_dme = XPLMGetDatai(datadefs.sim_cockpit_radios_adf2_has_dme);
	data.sim_cockpit_radios_dme5_has_dme = XPLMGetDatai(datadefs.sim_cockpit_radios_dme5_has_dme);
	data.sim_cockpit_radios_nav_com_adf_mode = XPLMGetDatai(datadefs.sim_cockpit_radios_nav_com_adf_mode);
	data.sim_cockpit_switches_RMI_l_vor_adf_selector = XPLMGetDatai(datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector);
	data.sim_cockpit_switches_RMI_l_vor_adf_selector2 = XPLMGetDatai(datadefs.sim_cockpit_switches_RMI_l_vor_adf_selector2);
	data.sim_cockpit_switches_RMI_r_vor_adf_selector = XPLMGetDatai(datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector);
	data.sim_cockpit_switches_RMI_r_vor_adf_selector2 = XPLMGetDatai(datadefs.sim_cockpit_switches_RMI_r_vor_adf_selector2);
	data.sim_operation_failures_rel_adf1 = XPLMGetDatai(datadefs.sim_operation_failures_rel_adf1);
	data.sim_operation_failures_rel_adf2 = XPLMGetDatai(datadefs.sim_operation_failures_rel_adf2);
	data.sim_operation_override_override_adf = XPLMGetDatai(datadefs.sim_operation_override_override_adf);
	data.sim_cockpit2_radios_actuators_adf1_power = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_power);
	data.sim_cockpit2_radios_actuators_adf2_power = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_power);
	data.sim_cockpit2_radios_actuators_adf1_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf2_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot);
	data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot);
	data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot);
	data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot);
	data.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot);
	data.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot);
	data.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot);
	data.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot);
	data.sim_cockpit2_radios_actuators_nav_com_adf_mode = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_nav_com_adf_mode);
	data.sim_cockpit2_radios_actuators_audio_nav_selection = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_audio_nav_selection);
	data.sim_cockpit2_radios_actuators_audio_selection_adf1 = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_audio_selection_adf1);
	data.sim_cockpit2_radios_actuators_audio_selection_adf2 = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_audio_selection_adf2);
	data.sim_cockpit2_radios_actuators_audio_volume_adf1 = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_audio_volume_adf1);
	data.sim_cockpit2_radios_actuators_audio_volume_adf2 = XPLMGetDataf(datadefs.sim_cockpit2_radios_actuators_audio_volume_adf2);
	data.sim_cockpit2_radios_actuators_adf1_left_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_left_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf2_left_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_left_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf1_right_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_right_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf2_right_frequency_hz = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_right_frequency_hz);
	data.sim_cockpit2_radios_actuators_adf1_right_is_selected = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf1_right_is_selected);
	data.sim_cockpit2_radios_actuators_adf2_right_is_selected = XPLMGetDatai(datadefs.sim_cockpit2_radios_actuators_adf2_right_is_selected);
	data.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag);
	data.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag);
	data.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg);
	data.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg);
	data.sim_cockpit2_radios_indicators_adf1_has_dme = XPLMGetDatai(datadefs.sim_cockpit2_radios_indicators_adf1_has_dme);
	data.sim_cockpit2_radios_indicators_adf2_has_dme = XPLMGetDatai(datadefs.sim_cockpit2_radios_indicators_adf2_has_dme);
	data.sim_cockpit2_radios_indicators_adf1_dme_distance_nm = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf1_dme_distance_nm);
	data.sim_cockpit2_radios_indicators_adf2_dme_distance_nm = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf2_dme_distance_nm);
	data.sim_cockpit2_radios_indicators_adf1_dme_speed_kts = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf1_dme_speed_kts);
	data.sim_cockpit2_radios_indicators_adf2_dme_speed_kts = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf2_dme_speed_kts);
	data.sim_cockpit2_radios_indicators_adf1_dme_time_min = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf1_dme_time_min);
	data.sim_cockpit2_radios_indicators_adf2_dme_time_min = XPLMGetDataf(datadefs.sim_cockpit2_radios_indicators_adf2_dme_time_min);
	XPLMGetDatab(datadefs.sim_cockpit2_radios_indicators_adf1_nav_id, datadefs.sim_cockpit2_radios_indicators_adf1_nav_id, 0, 500);
	XPLMGetDatab(datadefs.sim_cockpit2_radios_indicators_adf2_nav_id, datadefs.sim_cockpit2_radios_indicators_adf2_nav_id, 0, 500);
}

void xplane_print_new(const struct xplane_datadef_data_t &old_data, const struct xplane_datadef_data_t &new_data) {
	if (old_data.sim_cockpit_radios_adf1_freq_hz != new_data.sim_cockpit_radios_adf1_freq_hz) {
		fprintf(stderr, "sim/cockpit/radios/adf1_freq_hz: %u\n", new_data.sim_cockpit_radios_adf1_freq_hz);
	}
	if (old_data.sim_cockpit_radios_adf2_freq_hz != new_data.sim_cockpit_radios_adf2_freq_hz) {
		fprintf(stderr, "sim/cockpit/radios/adf2_freq_hz: %u\n", new_data.sim_cockpit_radios_adf2_freq_hz);
	}
	if (old_data.sim_cockpit_radios_adf1_stdby_freq_hz != new_data.sim_cockpit_radios_adf1_stdby_freq_hz) {
		fprintf(stderr, "sim/cockpit/radios/adf1_stdby_freq_hz: %u\n", new_data.sim_cockpit_radios_adf1_stdby_freq_hz);
	}
	if (old_data.sim_cockpit_radios_adf2_stdby_freq_hz != new_data.sim_cockpit_radios_adf2_stdby_freq_hz) {
		fprintf(stderr, "sim/cockpit/radios/adf2_stdby_freq_hz: %u\n", new_data.sim_cockpit_radios_adf2_stdby_freq_hz);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_dir_degt - new_data.sim_cockpit_radios_adf1_dir_degt) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_dir_degt: %.3f\n", new_data.sim_cockpit_radios_adf1_dir_degt);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_dir_degt - new_data.sim_cockpit_radios_adf2_dir_degt) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_dir_degt: %.3f\n", new_data.sim_cockpit_radios_adf2_dir_degt);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_dme_dist_m - new_data.sim_cockpit_radios_adf1_dme_dist_m) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_dme_dist_m: %.3f\n", new_data.sim_cockpit_radios_adf1_dme_dist_m);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_dme_dist_m - new_data.sim_cockpit_radios_adf2_dme_dist_m) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_dme_dist_m: %.3f\n", new_data.sim_cockpit_radios_adf2_dme_dist_m);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_dme_speed_kts - new_data.sim_cockpit_radios_adf1_dme_speed_kts) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_dme_speed_kts: %.3f\n", new_data.sim_cockpit_radios_adf1_dme_speed_kts);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_dme_speed_kts - new_data.sim_cockpit_radios_adf2_dme_speed_kts) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_dme_speed_kts: %.3f\n", new_data.sim_cockpit_radios_adf2_dme_speed_kts);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_dme_time_secs - new_data.sim_cockpit_radios_adf1_dme_time_secs) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_dme_time_secs: %.3f\n", new_data.sim_cockpit_radios_adf1_dme_time_secs);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_dme_time_secs - new_data.sim_cockpit_radios_adf2_dme_time_secs) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_dme_time_secs: %.3f\n", new_data.sim_cockpit_radios_adf2_dme_time_secs);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_cardinal_dir - new_data.sim_cockpit_radios_adf1_cardinal_dir) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_cardinal_dir: %.3f\n", new_data.sim_cockpit_radios_adf1_cardinal_dir);
	}
	if (fabs(old_data.sim_cockpit_radios_adf1_cardinal_dir2 - new_data.sim_cockpit_radios_adf1_cardinal_dir2) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf1_cardinal_dir2: %.3f\n", new_data.sim_cockpit_radios_adf1_cardinal_dir2);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_cardinal_dir - new_data.sim_cockpit_radios_adf2_cardinal_dir) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_cardinal_dir: %.3f\n", new_data.sim_cockpit_radios_adf2_cardinal_dir);
	}
	if (fabs(old_data.sim_cockpit_radios_adf2_cardinal_dir2 - new_data.sim_cockpit_radios_adf2_cardinal_dir2) > 0.1) {
		fprintf(stderr, "sim/cockpit/radios/adf2_cardinal_dir2: %.3f\n", new_data.sim_cockpit_radios_adf2_cardinal_dir2);
	}
	if (old_data.sim_cockpit_radios_adf1_has_dme != new_data.sim_cockpit_radios_adf1_has_dme) {
		fprintf(stderr, "sim/cockpit/radios/adf1_has_dme: %u\n", new_data.sim_cockpit_radios_adf1_has_dme);
	}
	if (old_data.sim_cockpit_radios_adf2_has_dme != new_data.sim_cockpit_radios_adf2_has_dme) {
		fprintf(stderr, "sim/cockpit/radios/adf2_has_dme: %u\n", new_data.sim_cockpit_radios_adf2_has_dme);
	}
	if (old_data.sim_cockpit_radios_dme5_has_dme != new_data.sim_cockpit_radios_dme5_has_dme) {
		fprintf(stderr, "sim/cockpit/radios/dme5_has_dme: %u\n", new_data.sim_cockpit_radios_dme5_has_dme);
	}
	if (old_data.sim_cockpit_radios_nav_com_adf_mode != new_data.sim_cockpit_radios_nav_com_adf_mode) {
		fprintf(stderr, "sim/cockpit/radios/nav_com_adf_mode: %u\n", new_data.sim_cockpit_radios_nav_com_adf_mode);
	}
	if (old_data.sim_cockpit_switches_RMI_l_vor_adf_selector != new_data.sim_cockpit_switches_RMI_l_vor_adf_selector) {
		fprintf(stderr, "sim/cockpit/switches/RMI_l_vor_adf_selector: %u\n", new_data.sim_cockpit_switches_RMI_l_vor_adf_selector);
	}
	if (old_data.sim_cockpit_switches_RMI_l_vor_adf_selector2 != new_data.sim_cockpit_switches_RMI_l_vor_adf_selector2) {
		fprintf(stderr, "sim/cockpit/switches/RMI_l_vor_adf_selector2: %u\n", new_data.sim_cockpit_switches_RMI_l_vor_adf_selector2);
	}
	if (old_data.sim_cockpit_switches_RMI_r_vor_adf_selector != new_data.sim_cockpit_switches_RMI_r_vor_adf_selector) {
		fprintf(stderr, "sim/cockpit/switches/RMI_r_vor_adf_selector: %u\n", new_data.sim_cockpit_switches_RMI_r_vor_adf_selector);
	}
	if (old_data.sim_cockpit_switches_RMI_r_vor_adf_selector2 != new_data.sim_cockpit_switches_RMI_r_vor_adf_selector2) {
		fprintf(stderr, "sim/cockpit/switches/RMI_r_vor_adf_selector2: %u\n", new_data.sim_cockpit_switches_RMI_r_vor_adf_selector2);
	}
	if (old_data.sim_operation_failures_rel_adf1 != new_data.sim_operation_failures_rel_adf1) {
		fprintf(stderr, "sim/operation/failures/rel_adf1: %u\n", new_data.sim_operation_failures_rel_adf1);
	}
	if (old_data.sim_operation_failures_rel_adf2 != new_data.sim_operation_failures_rel_adf2) {
		fprintf(stderr, "sim/operation/failures/rel_adf2: %u\n", new_data.sim_operation_failures_rel_adf2);
	}
	if (old_data.sim_operation_override_override_adf != new_data.sim_operation_override_override_adf) {
		fprintf(stderr, "sim/operation/override/override_adf: %u\n", new_data.sim_operation_override_override_adf);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_power != new_data.sim_cockpit2_radios_actuators_adf1_power) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_power: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_power);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_power != new_data.sim_cockpit2_radios_actuators_adf2_power) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_power: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_power);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf1_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf2_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_standby_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_standby_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_standby_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_standby_frequency_hz);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot - new_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_pilot: %.3f\n", new_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_pilot);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot - new_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_pilot: %.3f\n", new_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_pilot);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot - new_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_card_heading_deg_mag_copilot: %.3f\n", new_data.sim_cockpit2_radios_actuators_adf1_card_heading_deg_mag_copilot);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot - new_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_card_heading_deg_mag_copilot: %.3f\n", new_data.sim_cockpit2_radios_actuators_adf2_card_heading_deg_mag_copilot);
	}
	if (old_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot != new_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/RMI_left_use_adf_pilot: %u\n", new_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_pilot);
	}
	if (old_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot != new_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/RMI_left_use_adf_copilot: %u\n", new_data.sim_cockpit2_radios_actuators_RMI_left_use_adf_copilot);
	}
	if (old_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot != new_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/RMI_right_use_adf_pilot: %u\n", new_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_pilot);
	}
	if (old_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot != new_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/RMI_right_use_adf_copilot: %u\n", new_data.sim_cockpit2_radios_actuators_RMI_right_use_adf_copilot);
	}
	if (old_data.sim_cockpit2_radios_actuators_nav_com_adf_mode != new_data.sim_cockpit2_radios_actuators_nav_com_adf_mode) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/nav_com_adf_mode: %u\n", new_data.sim_cockpit2_radios_actuators_nav_com_adf_mode);
	}
	if (old_data.sim_cockpit2_radios_actuators_audio_nav_selection != new_data.sim_cockpit2_radios_actuators_audio_nav_selection) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/audio_nav_selection: %u\n", new_data.sim_cockpit2_radios_actuators_audio_nav_selection);
	}
	if (old_data.sim_cockpit2_radios_actuators_audio_selection_adf1 != new_data.sim_cockpit2_radios_actuators_audio_selection_adf1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/audio_selection_adf1: %u\n", new_data.sim_cockpit2_radios_actuators_audio_selection_adf1);
	}
	if (old_data.sim_cockpit2_radios_actuators_audio_selection_adf2 != new_data.sim_cockpit2_radios_actuators_audio_selection_adf2) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/audio_selection_adf2: %u\n", new_data.sim_cockpit2_radios_actuators_audio_selection_adf2);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_audio_volume_adf1 - new_data.sim_cockpit2_radios_actuators_audio_volume_adf1) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/audio_volume_adf1: %.3f\n", new_data.sim_cockpit2_radios_actuators_audio_volume_adf1);
	}
	if (fabs(old_data.sim_cockpit2_radios_actuators_audio_volume_adf2 - new_data.sim_cockpit2_radios_actuators_audio_volume_adf2) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/audio_volume_adf2: %.3f\n", new_data.sim_cockpit2_radios_actuators_audio_volume_adf2);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_left_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf1_left_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_left_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_left_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_left_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf2_left_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_left_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_left_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_right_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf1_right_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_right_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_right_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_right_frequency_hz != new_data.sim_cockpit2_radios_actuators_adf2_right_frequency_hz) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_right_frequency_hz: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_right_frequency_hz);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf1_right_is_selected != new_data.sim_cockpit2_radios_actuators_adf1_right_is_selected) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf1_right_is_selected: %u\n", new_data.sim_cockpit2_radios_actuators_adf1_right_is_selected);
	}
	if (old_data.sim_cockpit2_radios_actuators_adf2_right_is_selected != new_data.sim_cockpit2_radios_actuators_adf2_right_is_selected) {
		fprintf(stderr, "sim/cockpit2/radios/actuators/adf2_right_is_selected: %u\n", new_data.sim_cockpit2_radios_actuators_adf2_right_is_selected);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag - new_data.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_bearing_deg_mag: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf1_bearing_deg_mag);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag - new_data.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_bearing_deg_mag: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf2_bearing_deg_mag);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg - new_data.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_relative_bearing_deg: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf1_relative_bearing_deg);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg - new_data.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_relative_bearing_deg: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf2_relative_bearing_deg);
	}
	if (old_data.sim_cockpit2_radios_indicators_adf1_has_dme != new_data.sim_cockpit2_radios_indicators_adf1_has_dme) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_has_dme: %u\n", new_data.sim_cockpit2_radios_indicators_adf1_has_dme);
	}
	if (old_data.sim_cockpit2_radios_indicators_adf2_has_dme != new_data.sim_cockpit2_radios_indicators_adf2_has_dme) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_has_dme: %u\n", new_data.sim_cockpit2_radios_indicators_adf2_has_dme);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf1_dme_distance_nm - new_data.sim_cockpit2_radios_indicators_adf1_dme_distance_nm) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_dme_distance_nm: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf1_dme_distance_nm);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf2_dme_distance_nm - new_data.sim_cockpit2_radios_indicators_adf2_dme_distance_nm) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_dme_distance_nm: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf2_dme_distance_nm);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf1_dme_speed_kts - new_data.sim_cockpit2_radios_indicators_adf1_dme_speed_kts) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_dme_speed_kts: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf1_dme_speed_kts);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf2_dme_speed_kts - new_data.sim_cockpit2_radios_indicators_adf2_dme_speed_kts) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_dme_speed_kts: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf2_dme_speed_kts);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf1_dme_time_min - new_data.sim_cockpit2_radios_indicators_adf1_dme_time_min) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_dme_time_min: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf1_dme_time_min);
	}
	if (fabs(old_data.sim_cockpit2_radios_indicators_adf2_dme_time_min - new_data.sim_cockpit2_radios_indicators_adf2_dme_time_min) > 0.1) {
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_dme_time_min: %.3f\n", new_data.sim_cockpit2_radios_indicators_adf2_dme_time_min);
	}
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf1_nav_id: ");
		for (int i = 0; i < 10; i++) {
			fprintf(stderr, "%02x", new_data.sim_cockpit2_radios_indicators_adf1_nav_id[i]);
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "sim/cockpit2/radios/indicators/adf2_nav_id: ");
		for (int i = 0; i < 10; i++) {
			fprintf(stderr, "%02x", new_data.sim_cockpit2_radios_indicators_adf2_nav_id[i]);
		}
		fprintf(stderr, "\n");
}
