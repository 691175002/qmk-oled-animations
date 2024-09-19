/* settings.c
 *
 * Manages user settings including OLED brightness, tap delays, and their storage in EEPROM.
 * Supports loading and saving settings to ensure custom configurations persist across resets.
 *
 * Author: Ryan Turner
 */

#include <settings.h>

// Runs if EEPROM has been reset
void eeconfig_init_user(void) {
	user_config.raw = 0;
	
	oled_state = 0;
	oled_show_info = true;
	oled_bri = OLED_BRIGHTNESS;
	
	delay_base = TAP_DELAY_BASE;
	delay_ctrl = TAP_DELAY_CTRL;
	delay_bksp = TAP_DELAY_BKSP;

	save_settings();
}

void keyboard_post_init_user(void) {
	load_settings();
}

void load_settings(void) {
	user_config.raw = eeconfig_read_user();
	
	delay_base = user_config.delay_base_div5 * 5;
	delay_ctrl = user_config.delay_ctrl_div5 * 5;
	delay_bksp = user_config.delay_bksp_div5 * 5;
	
	oled_state = user_config.oled_state;
	oled_show_info = user_config.oled_show_info;
	oled_bri = user_config.oled_bri_div10 * 10;
	
	oled_set_brightness (oled_bri);
}

void save_settings(void) {
	user_config.delay_base_div5 = delay_base / 5;
	user_config.delay_ctrl_div5 = delay_ctrl / 5;
	user_config.delay_bksp_div5 = delay_bksp / 5;
	
	user_config.oled_state = oled_state;
	user_config.oled_show_info = oled_show_info;
	user_config.oled_bri_div10 = oled_bri / 10;
	
	eeconfig_update_user(user_config.raw);
}

// Helper function to adjust a setting within specified boundaries
void adjust_setting(uint16_t *setting, int16_t delta, uint16_t min, uint16_t max) {
	if (delta > 0) {
		*setting = (*setting <= (max - delta)) ? *setting + delta : max;
	} else {
		*setting = (*setting >= (min - delta)) ? *setting + delta : min;
	}
}
