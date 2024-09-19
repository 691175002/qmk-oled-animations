/* keymap.c
 * 
 * Highly customized Odin75 keymap. I have tried to keep the keymap as configurable and modular as
 * possible. Many #include features would be easy to integrate into a new keymap or keyboard as well.
 *
 * Requires the following features in rules.mk:
 *
 * DEFERRED_EXEC_ENABLE = yes
 * DYNAMIC_MACRO_ENABLE = yes
 * MOUSEKEY_ENABLE = yes
 * TAP_DANCE_ENABLE = yes
 * SEND_STRING_ENABLE = yes
 *
 * Author: Ryan Turner
 */

#include QMK_KEYBOARD_H
#include "keymap.h"
#include "settings.h"
#include "features/bitmaps.h"
#include "features/special_tap_dance.h"
#include "features/dynamic_macro_status.h"
#include "features/send_string_macros.h"
#include "features/mouse_jiggler.h"
#include "features/select_word.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Todo: Fix scrolling autostop.  Call it mag?

// ========================
// = Keymap Configuration =
// ========================
enum {
	LAYER_BASE   = 0,
	LAYER_NUMPAD = 1,
	LAYER_FUNC   = 2,
	LAYER_MACRO  = 3,
	LAYER_CONFIG = 4,
	LAYER_MOUSE  = 5,
	LAYER_ENUM_COUNT
};

/* The name of the highest active layer will be displayed on the OLED. If the layer is marked as a prefix
 * its name will always be displayed as a prefix when active, even if it is not the top layer.
 */
layer_info_t layer_info[] = {
    [LAYER_BASE]   = { .name = "Base",  .prefix = false },
    [LAYER_NUMPAD] = { .name = "Num",   .prefix = true  },
    [LAYER_FUNC]   = { .name = "Func",  .prefix = false },
    [LAYER_MACRO]  = { .name = "Macro", .prefix = false },
    [LAYER_CONFIG] = { .name = "Confg", .prefix = false },
    [LAYER_MOUSE]  = { .name = "Mou",   .prefix = true  },
};

enum custom_keycodes {
/* Tap Dance keycodes register different key or layer actions depending on whether they are tapped or held.
 * Up to four distinct actions can be easily configured using ACTION_TAP_DANCE_TRI or QUAD.
 * See https://docs.qmk.fm/#/feature_tap_dance and special_tap_dance.h for details.
 * These keycodes start at 0 and are configured in tap_dance_actions. */
	BKSP_BSL, LALT_PRV, RALT_NXT, ESC_FUCK,
	CAPS_IME, CTRL_OCR, CTRL_RGT, GUI_SNIP,
	TD_FN_LH, TD_F13_L, TD_HOME,  TD_END,
	TD_PGUP,  TD_PGDN,
	
/* Custom Keycodes are for behaviors that require new code to implement. Creating a custom keycode is more
 * complex than a Tap Dance or Macro, so consider whether those simpler features will work first.
 * These keycodes start at SAFE_RANGE and are configured in process_record_user. */
	LED_BUP = SAFE_RANGE,
	LED_BDN, LED_ANI, LED_INF,
	BASE_UP, BASE_DN,
	CTRL_UP, CTRL_DN,
	BKSP_UP, BKSP_DN,
	EROM_SV, EROM_LD,
	M_JIGGL, SELWORD,
	
/* Macro Keycodes perform a single SEND_STRING command when pressed and also display their type and name
 * on the OLED display when activated. They are useful for macros and keyboard shortcuts ranging from 
 * simple to extrordinarily complex. See https://docs.qmk.fm/#/feature_send_string for details.
 * These keycodes start at MACRO_RANGE_START and are configured in macro_info */
	MACRO_RANGE_START,
	M_AUTHR,
	M_TWTCH,
	MPDF_SV,
	MPDF_1P,
	MPDF_2P,
	MPDF_CT,
	MACRO_RANGE_END
};

#define TD_TAB_LAYER LT(LAYER_MACRO, KC_TAB)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT( /* Layer 0: Base Layer */
	TD(ESC_FUCK), KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  TD(TD_F13_L),  KC_INS,  KC_DEL,
	KC_GRV,       KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,       TD(TD_HOME), TD(TD_PGUP),
	TD_TAB_LAYER, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, TD(BKSP_BSL),  TD(TD_END),  TD(TD_PGDN),
	TD(CAPS_IME), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
	KC_LSFT,      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          TD(TD_FN_LH),  KC_UP,
	KC_LCTL,      TD(GUI_SNIP), TD(LALT_PRV),                  KC_SPC,                            TD(RALT_NXT), TD(CTRL_RGT), KC_LEFT,       KC_DOWN, KC_RGHT
  ),
  
  [LAYER_NUMPAD] = LAYOUT( /* Numpad Overlay:  Toggled by Double Tap or Hold on TD_F13_L */
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, KC_KP_7, KC_KP_8, KC_KP_9, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, KC_KP_4, KC_KP_5, KC_KP_6, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, KC_KP_1, KC_KP_2, KC_KP_3, _______, _______,          _______,     
	_______,      _______, _______, _______, _______, _______, _______, KC_KP_0, _______, KC_PDOT, KC_PENT, _______,          _______,       _______,
	_______,      _______, _______,                            _______,                                     _______, _______, _______,       _______, _______
  ),

  [LAYER_FUNC] = LAYOUT( /* Function Layer: Activated by Hold on CAPS or FN */
	KC_ESC,       C(KC_F1), C(KC_F2), C(KC_F3), C(KC_F4), C(KC_F5), C(KC_F6), C(KC_F7), C(KC_F8), C(KC_F9), C(KC_F10), C(KC_F11), C(KC_F12), _______, DM_PLY1, DM_PLY2,
	_______,      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_BSPC,       KC_HOME, KC_PGUP,
	_______,      _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, KC_NUM,  KC_SCRL, KC_BSLS,       KC_END,  KC_PGDN,
	_______,      KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, KC_PSCR, KC_PAUS,          _______,
	KC_LSFT,      KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, SELWORD,          _______,       KC_MPLY,
	KC_LCTL,      KC_LGUI, C(S(KC_TAB)),                       _______,                                LCTL(KC_TAB), KC_RCTL, KC_MPRV,       KC_MSTP, KC_MNXT
  ),
  
  [LAYER_MACRO] = LAYOUT( /* Macro Layer: Activated by Hold on TAB */
	_______,      C(KC_F13), C(KC_F14), C(KC_F15), C(KC_F16), C(KC_F17), C(KC_F18), C(KC_F19), C(KC_F20), C(KC_F21), C(KC_F22), C(KC_F23), C(KC_F24), DM_RSTP, DM_REC1, DM_REC2,
	_______,      KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  _______,       LED_ANI, LED_BUP,
	_______,      _______, _______, _______, M_TWTCH, _______, _______, _______, _______, _______, _______, _______, _______, _______,       LED_INF, LED_BDN,
	_______,      MPDF_1P, MPDF_2P, MPDF_CT, MPDF_SV, _______, _______, _______, _______, _______, _______, _______,          M_AUTHR,     
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, M_JIGGL,          _______,       _______,
	KC_LCTL,      KC_LGUI, KC_LALT,                            _______,                                     KC_RALT, KC_RCTL, _______,       _______, _______
  ),
  
  [LAYER_CONFIG] = LAYOUT( /* Config Layer: Activated by Double-Hold on CAPS */
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,       LED_ANI, LED_BUP,
	_______,      BASE_UP, CTRL_UP, BKSP_UP, _______, _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,        LED_INF, LED_BDN,
	_______,      BASE_DN, CTRL_DN, BKSP_DN, _______, _______, _______, _______, _______, _______, _______, _______,          EROM_SV,
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, EROM_LD,          _______,       _______,
	_______,      _______, _______,                            _______,                                     _______, _______, _______,       _______, _______
  ),

  [LAYER_MOUSE] = LAYOUT( /* Mouse Overlay: Toggled by Single Tap or Hold on TD_F13_L */
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,       _______, _______,
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_SPC,     
	_______,      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN3,          _______,       _______,
	_______,      _______, _______,                            KC_ENT,                                      KC_BTN1, KC_BTN2, _______,       _______, _______
  ),
};

// ======================
// = OLED Configuration =
// ======================
// Configure the display states and their order here, implementations are in oled_task_user
enum {
	OLED_TOTORO, OLED_NEKO, OLED_GHOST, OLED_WHALE, OLED_GIRL, OLED_DEMON, OLED_MAI,
	OLED_FACES,  OLED_CAT, OLED_CHARACTERS,
	OLED_ENUM_COUNT
};

// The initial value of the buffer will be displayed when the keyboard is first powered on
char show_buffer[SHOW_LEN] =
	"\n\n"
	"   \x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\n"
	"   \xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\n"
	"   \xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\n";

static uint32_t note_timer = 0;

// =========================
// = Keycode Configuration =
// =========================
// Tap Dance actions can be configured using the ACTION_TAP_DANCE macros.
tap_dance_action_t tap_dance_actions[] = {
	[BKSP_BSL] = ACTION_TAP_DANCE_TAP_HOLD(LCTL(KC_BSPC), KC_BSLS),
	[LALT_PRV] = ACTION_TAP_DANCE_TAP_HOLD(LCTL(KC_PGUP), KC_LALT),
	[RALT_NXT] = ACTION_TAP_DANCE_TAP_HOLD(LCTL(KC_PGDN), KC_RALT),
	
	[ESC_FUCK] = ACTION_TAP_DANCE_QUAD(KC_ESC,     KC_ESC,     TDM_KC, C(S(KC_ESC)), TDM_KC, C(A(KC_DEL)), TDM_KC),
	[CAPS_IME] = ACTION_TAP_DANCE_QUAD(C(KC_GRV),  LAYER_FUNC, TDM_LH, KC_CAPS,      TDM_KC, LAYER_CONFIG, TDM_LH),
//  [CTRL_OCR] = ACTION_TAP_DANCE_TRI (KC_LCTL,    KC_LCTL,    TDM_KC, LSG(KC_T),    TDM_KC),
	[CTRL_RGT] = ACTION_TAP_DANCE_TRI (C(KC_L),    KC_RCTL,    TDM_KC, LSG(KC_C),    TDM_KC),
	[GUI_SNIP] = ACTION_TAP_DANCE_QUAD(KC_LGUI,    KC_LGUI,    TDM_KC, LSG(KC_S),    TDM_KC, LSG(KC_T),    TDM_KC),
	[TD_FN_LH] = ACTION_TAP_DANCE_QUAD(KC_F11,     LAYER_FUNC, TDM_LH, KC_MYCM,      TDM_KC, LAYER_MACRO,  TDM_LH),
	[TD_F13_L] = ACTION_TAP_DANCE_QUAD_FULL(LAYER_MOUSE, TDM_LT, LAYER_MOUSE, TDM_LH, LAYER_NUMPAD, TDM_LT, LAYER_NUMPAD, TDM_LH),
	
	[TD_HOME]  = ACTION_TAP_DANCE_DOUBLE(KC_HOME, LCTL(LSFT(KC_T))),
	[TD_END]   = ACTION_TAP_DANCE_DOUBLE(KC_END,  LCTL(KC_W)),
	[TD_PGUP]  = ACTION_TAP_DANCE_DOUBLE(KC_PGUP, LCTL(KC_EQL)),
	[TD_PGDN]  = ACTION_TAP_DANCE_DOUBLE(KC_PGDN, LCTL(KC_MINS)),
};

// Macros are configured as a SEND_STRING along with a type and name that are displayed when run
macro_info_t macro_info[] = {
	[M(M_AUTHR)] = MACRO("Print", "Author Name", "Keymap Author: Ryan Turner\n"),
	[M(M_TWTCH)] = MACRO("Web", "Twitch.tv", SS_START("firefox") SS_LCTL("l") "twitch.tv/directory/category/starcraft" SS_LCTL("\n")),
	
	[M(MPDF_SV)] = MACRO("Adobe PDF", "Save to Desktop", SS_LCTL(SS_LSFT("s")) SS_DELAY(300) REPT(SS_TAP(X_TAB), 5)
														 SS_TAP(X_ENT) SS_DELAY(300) SS_LCTL("l") "Desktop" REPT(SS_TAP(X_ENT), 3)),
	[M(MPDF_1P)] = MACRO("Adobe PDF", "1-Page View",     SS_LALT("vps")),
	[M(MPDF_2P)] = MACRO("Adobe PDF", "2-Page View",     SS_LALT("vpp")),
	[M(MPDF_CT)] = MACRO("Adobe PDF", "Toggle Cover",    SS_LALT("vpv")),
};

// Custom keycodes that do not fall into the above categories can be implemented here
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (!process_select_word(keycode, record, SELWORD)) { return false; }
	if (record->event.pressed) {
		switch (keycode) {
			// OLED Config
			case LED_BUP:
				adjust_setting(&oled_bri, 10, 0, 250); // Do not change constants as eeprom packs these into 6 bits
				oled_set_brightness (oled_bri);
				break;
				
			case LED_BDN:
				adjust_setting(&oled_bri, -10, 0, 250);
				oled_set_brightness (oled_bri);
				break;
				
			case LED_ANI:
				if (oled_enabled) {
					if (oled_show_info) {
						oled_show_info = false;
					} else {
						oled_state = (oled_state + 1) % OLED_ENUM_COUNT;
					}
				} else {
					oled_enabled = true;
					oled_show_info = false;
				}
				break;
				
			case LED_INF:
				if (oled_enabled) {
					if (oled_show_info) {
						oled_enabled = false;
					} else {
						oled_show_info = true;
					}
				} else {
					oled_enabled = true;
					oled_show_info = true;
				}
				break;
			
			// Tap Delay Config
			case BASE_UP: adjust_setting(&delay_base,  5, 100, 320); break; // Do not change constants as eeprom packs these into 6 bits
			case BASE_DN: adjust_setting(&delay_base, -5, 100, 320); break; // Zeroing delay_base can lock you out of LAYER_CONFIG
			case CTRL_UP: adjust_setting(&delay_ctrl,  5, 10,  320); break;
			case CTRL_DN: adjust_setting(&delay_ctrl, -5, 10,  320); break;
			case BKSP_UP: adjust_setting(&delay_bksp,  5, 10,  320); break;
			case BKSP_DN: adjust_setting(&delay_bksp, -5, 10,  320); break;
			
			// EEPROM
			case EROM_SV:
				show_feature("EEPROM", "Save Config");
				save_settings();
				break;
				
			case EROM_LD:
				show_feature("EEPROM", "Load Config");
				load_settings();
				break;
			
			// Mouse Jiggler
			case M_JIGGL:
				if (!jiggle_state()) {
					show_feature("Mouse Jiggle", "Enable");
					jiggle_start();
				} else {
					show_feature("Mouse Jiggle", "Disable");
					jiggle_stop();
				}
				break;
				
			default:
				/* Macros configured using macro_info are processed here.
				 * send_string is called using defer_exec to allow the OLED to update first,
				 * otherwise macros with delays will cause the display to freeze.
				 */
				static int macro_index = -1; // Malloc would allow simultaneous presses but seems unnecessary
				
				if ((keycode > MACRO_RANGE_START) && (keycode < MACRO_RANGE_END) && (macro_index == -1)) {
					macro_index = M_INDEX(keycode);
					show_macro(macro_info[macro_index].type, macro_info[macro_index].name);
					
					uint32_t send_string_deferred(uint32_t trigger_time, void *cb_arg) {
						send_string_with_delay(macro_info[macro_index].macro, DYNAMIC_MACRO_DELAY);
						macro_index = -1;
						return 0;
					}
					defer_exec((1000 / OLED_FPS) + 1, send_string_deferred, NULL);
					break;
				}
				break;
		}
	}
	return true;
}

// =============
// = Tap Delay =
// =============
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(CTRL_OCR):
            return delay_ctrl;
		case TD(CTRL_RGT):
			return delay_ctrl;
		case TD(BKSP_BSL):
			return delay_bksp;
        default:
            return delay_base;
    }
}

// ================
// = OLED Display =
// ================
// Does some housekeeping for oled_task_user
bool oled_task_prep(void) {
	static uint32_t frame_timer = 0;
	static uint16_t oled_last_state = 0;
	
	if (timer_elapsed32(frame_timer) < (1000 / OLED_FPS)) {
		return false;
	} else {
		frame_timer = timer_read32();
	}
	
	if (!oled_enabled) {
		oled_off();
		return false;
	}
	
	if (oled_last_state != oled_show_info) {
		oled_clear();
		oled_last_state = oled_show_info;
	}
	
	return true;
}

// OLED Display is implemented here
bool oled_task_user(void) {
	if (!oled_task_prep()) {
		return false;
	}
	
	static int fp_wpm_ema = 0;
	fp_wpm_ema = (((int)get_current_wpm() * FP_DIV * WPM_EMA_ALPHA) + (fp_wpm_ema * (FP_DIV - WPM_EMA_ALPHA))) / FP_DIV;
	
	if (timer_elapsed32(note_timer) <= OLED_NOTE_TIME) {
		// ==================
		// = Display Notice =
		// ==================
		// If there is a notice it will have already been written to the buffer
		oled_write_ln(show_buffer, false);
		
	} else if (oled_show_info) {
		// ==================
		// = Display Status =
		// ==================
		snprintf(show_buffer, SHOW_LEN, "\n Layer: ");
		
		int highest_layer = 0;
		for (int i = 0; i < LAYER_ENUM_COUNT; i++) {
			if (layer_state_is(i)) {
				if (layer_info[i].prefix) {
					snprintf_append(show_buffer, SHOW_LEN, "%s\x07", layer_info[i].name);
				} else {
					highest_layer = i;
				}
			}
		}
		
		// If the string is 22 characters long it will wrap on its own and does not need a newline
		if (snprintf_append(show_buffer, SHOW_LEN, "%s", layer_info[highest_layer].name) < 22) {
			snprintf_append(show_buffer, SHOW_LEN, "\n");
		}

		// Show keyboard state unless we are in the config layer where we show EEPROM settings instead
		if (!layer_state_is(LAYER_CONFIG)) {
			snprintf_append(show_buffer, SHOW_LEN,
				(dm_state_1 == DM_SET) ? " Macro1:%s %d\n" : " Macro1:%s\n",
				(dm_state_1 == DM_SET) ? "Set" : (dm_state_1 == DM_EMPTY) ? "Empty" : "Recording",
				dm_size_1);
			
			snprintf_append(show_buffer, SHOW_LEN,
				(dm_state_2 == DM_SET) ? " Macro2:%s %d\n" : " Macro2:%s\n",
				(dm_state_2 == DM_SET) ? "Set" : (dm_state_2 == DM_EMPTY) ? "Empty" : "Recording",
				dm_size_2);
		
			if (is_caps_word_on()) {
				snprintf_append(show_buffer, SHOW_LEN, " State: CAPS_WORD\n");
			} else {
				led_t led_state = host_keyboard_led_state();
				snprintf_append(show_buffer, SHOW_LEN, " State: %s%s%s\n",
					(led_state.caps_lock)   ? "CAP " : "",
					(led_state.num_lock)    ? "NUM " : "",
					(led_state.scroll_lock) ? "SCR " : "");
			}
			
			snprintf_append(show_buffer, SHOW_LEN, "\n Speed: %dwpm", fp_wpm_ema / FP_DIV);

		} else {
			snprintf_append(show_buffer, SHOW_LEN, " Base Delay: %dms\n", delay_base);
			snprintf_append(show_buffer, SHOW_LEN, " Ctrl Delay: %dms\n", delay_ctrl);
			snprintf_append(show_buffer, SHOW_LEN, " Bksp Delay: %dms\n", delay_bksp);
			snprintf_append(show_buffer, SHOW_LEN, "\n Brightness: %d/250", oled_bri);
		}
		
		oled_write_ln(show_buffer, false);
		
	} else {
		// ===============
		// = Image Modes =
		// ===============
		// Keep a moving average of the WPM to make animations smoother
		static int fp_position = 0;
		static int fp_velocity = 0;
		static int fp_target = 0;
		
		// Scale WPM into the range 0-64 so that it can be used as a vertical pixel count
		uint16_t mask = scale_value_lim(fp_wpm_ema / FP_DIV, WPM_MIN, WPM_MAX, 0, 64);
		
		if (oled_state == OLED_TOTORO) {
			split_render(64 - mask, TOTORO_FULL, TOTORO_FRONT);
			
		} else if (oled_state == OLED_NEKO) {
			split_render(mask, NEKO_FULL, NEKO_FRONT);
			
		} else if (oled_state == OLED_GHOST) {
			split_render(mask, GHOST_FULL, GHOST_FRONT);
			
		} else if (oled_state == OLED_WHALE) {
			split_render(64 - mask, WHALE_FRONT, WHALE_FULL);
			
		} else if (oled_state == OLED_GIRL) {
			split_render(mask, GIRL_TWO, GIRL_ONE);	
			
		} else if (oled_state == OLED_DEMON) {
			split_render(mask, DEMON_INV, DEMON);	
			
		} else if (oled_state == OLED_MAI) {
			split_render(64 - mask, MAI_FRONT, MAI_FULL);	
			
		} else if (oled_state == OLED_FACES) {
			// Scroll using fp_wpm_ema / WPM_DIV as velocity
			fp_position += fp_wpm_ema / WPM_DIV;
			fp_position = (fp_position + FACES_WIDTH * FP_DIV) % (FACES_WIDTH * FP_DIV);
			h_scroll_render(fp_position / FP_DIV, FACES, FACES_WIDTH);
			
		} else if (oled_state == OLED_CAT) {
			// Reverse direction when stopped
			static int direction = 1;
			static bool stopped = true;
			
			if (fp_wpm_ema < FP_DIV && stopped == false) {
				direction = direction * -1;
				stopped = true;
			} else if (fp_wpm_ema >= FP_DIV) {
				stopped = false;
			}
			
			fp_position += fp_wpm_ema / WPM_DIV * direction;
			fp_position = (fp_position + CAT_WIDTH * FP_DIV) % (CAT_WIDTH * FP_DIV);
			h_scroll_render(fp_position / FP_DIV, CAT_SCROLL, CAT_WIDTH);
			
		} else if (oled_state == OLED_CHARACTERS) {
			// Set velocity to max(vel, wpm) for fast response
			fp_velocity = (ABS(fp_velocity) > ABS(fp_wpm_ema / (WPM_DIV))) ? fp_velocity : (fp_wpm_ema / WPM_DIV);
			
			fp_target = closestMultiple(fp_position, 104 * FP_DIV);
			if (fp_wpm_ema < FP_DIV) {
				// User is not typing, slow down and pull towards nearest target
				fp_velocity += (50 * SIGN(fp_target - fp_position));
				fp_velocity -= (50 * fp_velocity) / FP_DIV;
			} else {
				// User is typing
				fp_velocity -= (10 * fp_velocity) / FP_DIV;
			}
			
			if ((ABS(fp_position - fp_target) <= FP_DIV) && (ABS(fp_velocity) <= 100)) {
				// Stop exactly at target if we are close
				fp_position = fp_target;
				fp_velocity = 0;
			} else {
				// Otherwise do a normal position update
				fp_position += fp_velocity;
				fp_position = (fp_position + CHARACTERS_HEIGHT * FP_DIV) % (CHARACTERS_HEIGHT * FP_DIV);
			}
			v_scroll_render(fp_position / FP_DIV, CHARACTERS, CHARACTERS_HEIGHT);
		}
	}
    return false;
}

void show_macro(const char* type, const char* name) {
	note_timer = timer_read32();
	snprintf(show_buffer, SHOW_LEN, "\n Send Macro\n Type\x1A%s\n Name\x1A%s", type, name);
	oled_clear();
}

void show_feature(const char* type, const char* name) {
	note_timer = timer_read32();
	snprintf(show_buffer, SHOW_LEN, "\n Feature\n Type\x1A%s\n Name\x1A%s", type, name);
	oled_clear();
}

// Convenience function that appends to a string using printf.
// This is less efficient than tracking the string length manually but it makes things simpler.
int snprintf_append(char *buffer, size_t bufferSize, const char *format, ...) {
    va_list args;
	va_start(args, format);
	
    size_t offset = strlen(buffer);
    int charsWritten = vsnprintf(buffer + offset, bufferSize - offset, format, args) + offset;
	
    va_end(args);
	
    return charsWritten;
}

// Scales value from the range min_in, max_in to min_out, max_out.
uint16_t scale_value_lim(uint16_t value, uint16_t min_in, uint16_t max_in, uint16_t min_out, uint16_t max_out) {
    if (value <= min_in) {
		return min_out;
	} else if (value >= max_in) {
		return max_out;
	} else {
		return ((value - min_in) * (max_out - min_out) / (max_in - min_in)) + min_out;
	}
}

int closestMultiple(int n, int m) {
    return n % m <= m / 2 ? n - (n % m) : n + (m - (n % m));
}