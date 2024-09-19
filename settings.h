/* settings.h
 *
 * Header file defining structures and functions for managing user settings in keyboard firmware.
 * Includes definitions for settings like OLED brightness and tap delays, and prototypes for functions to manipulate these settings.
 *
 * Sections:
 * - User Configuration Structure: Defines a union for easy access to individual settings stored in EEPROM.
 * - Settings Prototypes: Declarations for functions related to settings initialization, loading, saving, and adjustment.
 */
#pragma once

#include "quantum.h"

// For some reason the last few bits of user_config_t don't work so I leave 4 unused.
// Variables may be stored after dividing by 5 or 10 so that they fit in only 6 bits
typedef union {
	uint32_t raw;
	struct {
		uint16_t oled_state      : 4;
		uint16_t oled_bri_div10  : 6;
		uint16_t delay_base_div5 : 6;
		uint16_t delay_ctrl_div5 : 6;
		uint16_t delay_bksp_div5 : 6;
		bool 	 oled_show_info  : 1;
	};
} user_config_t;

user_config_t user_config;

// OLED state and brightness
bool oled_show_info;
uint16_t oled_state;
uint16_t oled_bri;

// Tap delays for use in get_tapping_term
uint16_t delay_base;
uint16_t delay_ctrl;
uint16_t delay_bksp;

void eeconfig_init_user(void);
void keyboard_post_init_user(void);

void load_settings(void);
void save_settings(void);

void adjust_setting(uint16_t *setting, int16_t delta, uint16_t min, uint16_t max);
