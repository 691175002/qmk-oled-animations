/* special_tap_dance.c
 *
 * Enhances QMK's tap dance feature by providing functions for complex tap dance actions.
 * Supports custom actions based on the tap count and whether the key is held, including tap-hold and quad tap dance functionalities.
 *
 * Requires the following features in rules.mk
 * TAP_DANCE_ENABLE = yes
 *
 * I also recommend #define PERMISSIVE_HOLD in config.h
 *
 * Author: Ryan Turner
 */

#include "special_tap_dance.h"
#include "quantum.h"

// ============
// = Tap Hold =
// ============
// Implementations for tap or hold actions
void tap_dance_tap_hold_on_each_release(tap_dance_state_t *state, void *user_data) {
	tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
	
	if (state->count && !state->finished) {
		tap_code16(tap_hold->tap);
	}
}

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (state->pressed) {
        if (state->count == 1) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

// ==================
// = Quad Tap Dance =
// ==================
// Definitions and handling for up to four different actions based on tap count and press state
typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
	TD_DOUBLE_HOLD,
	TD_MULTI_TAP,
	TD_MULTI_HOLD,
} td_state_t;

static td_state_t td_state;

td_state_t cur_dance(tap_dance_state_t  *state) {
    if (state->count == 1) {
		if (!state->pressed) {
			return TD_SINGLE_TAP;
		} else {
			return TD_SINGLE_HOLD;
		}
    } else if (state->count == 2) {
		if (!state->pressed) {
			return TD_DOUBLE_TAP;
		} else {
			return TD_DOUBLE_HOLD;
		}
	} else {
		if (!state->pressed) {
			return TD_MULTI_TAP;
		} else {
			return TD_MULTI_HOLD;
		}
	}
}

void finish_action(uint16_t kc_or_layer, tap_dance_mode_t mode) {
	switch (mode) {
		case TDM_KC:
			register_code16(kc_or_layer);
			break;
		case TDM_LH:
			layer_on(kc_or_layer);
			break;
		case TDM_LT:
			// Toggle will occur on reset
			break;
		default: break;
	}
}

void tap_dance_quad_finished(tap_dance_state_t *state, void *user_data) {
	tap_dance_quad_t *quad = (tap_dance_quad_t *)user_data;
	td_state = cur_dance(state);
	
    switch (td_state) {
        case TD_SINGLE_HOLD:
			finish_action(quad->single_hold, quad->single_hold_action);
			break;
        case TD_DOUBLE_HOLD:
			finish_action(quad->double_hold, quad->double_hold_action);
			break;
		default: break;
    }
}

void reset_action(uint16_t kc_or_layer, tap_dance_mode_t mode) {
	switch (mode) {
		case TDM_KC:
			switch (td_state) {
				case TD_SINGLE_TAP:  tap_code16(kc_or_layer); break;
				case TD_SINGLE_HOLD: unregister_code16(kc_or_layer); break;
				case TD_DOUBLE_TAP:  tap_code16(kc_or_layer); break;
				case TD_DOUBLE_HOLD: unregister_code16(kc_or_layer); break;
				default: break;
			}
			break;
		case TDM_LH:
			layer_off(kc_or_layer);
			break;
		case TDM_LT:
			layer_invert(kc_or_layer);
			break;
		default: break;
	}
}	

void tap_dance_quad_reset(tap_dance_state_t *state, void *user_data) {
	tap_dance_quad_t *quad = (tap_dance_quad_t *)user_data;

    switch (td_state) {
		case TD_SINGLE_TAP:
			reset_action(quad->single_tap, quad->single_tap_action);
			break;
		case TD_SINGLE_HOLD:
			reset_action(quad->single_hold, quad->single_hold_action);
			break;
		case TD_DOUBLE_TAP:
			reset_action(quad->double_tap, quad->double_tap_action);
			break;
		case TD_DOUBLE_HOLD:
			reset_action(quad->double_hold, quad->double_hold_action);
			break;
		default: break;
    }
}
