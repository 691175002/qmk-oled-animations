/* special_tap_dance.h
 *
 * Header file for advanced tap dance actions in keyboard firmware.
 * Includes type definitions and function prototypes for custom tap dance actions, facilitating complex key behavior customization.
 *
 * Based on heavily modified examples from https://docs.qmk.fm/#/feature_tap_dance.
 *   Example 3: Tap Hold
 *   Example 4: Quad Action
 *
 * Author: Ryan Turner
 */

#pragma once

#include "quantum.h"

typedef enum {
	TDM_NO,
	TDM_KC, // Keycode
	TDM_LH, // Layer Hold
	TDM_LT, // Layer Toggle
} tap_dance_mode_t;

typedef struct {
    uint16_t tap;
    uint16_t hold;
    uint16_t held;
} tap_dance_tap_hold_t;

typedef struct {
	uint16_t single_tap;
	uint16_t single_hold;
	uint16_t double_tap;
	uint16_t double_hold;
	
	tap_dance_mode_t single_tap_action;
	tap_dance_mode_t single_hold_action;
	tap_dance_mode_t double_tap_action;
	tap_dance_mode_t double_hold_action;
} tap_dance_quad_t;

/* Tap Hold
 * Registers tap when tapped, and hold when held.
 * This macro registers keycodes as early as possible using the release callback and then re-arms instead of
 * continuing to process events.  This makes it more responsive than ACTION_TAP_DANCE_QUAD when only two states
 * are needed.
 */
#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) \
    { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset, tap_dance_tap_hold_on_each_release}, \
	  .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }


/* Quad Tap Dance
 * Performs one of four actions depending on number of presses and whether the key is tapped or held.
 * Each scenario can either register a key or perform a layer hold/toggle operation.  Behavior is configured for
 * each action using the TDM enum:
 *
 * single_tap_action is performed on the keycode or layer passed to single_tap
 * single_hold_action is performed on the keycode or layer passed to single_hold
 * double_tap_action is performed on the keycode or layer passed to double_tap
 * double_hold_action is performed on the keycode or layer passed to double_hold
 *
 * QUAD_FULL requires all eight arguments. Simplified versions which use fewer arguments are also available.
 */
#define ACTION_TAP_DANCE_TRI(single_tap, single_hold, single_hold_action, double_tap, double_tap_action) \
    { .fn = {NULL, tap_dance_quad_finished, tap_dance_quad_reset, NULL}, \
	  .user_data = (void *)&((tap_dance_quad_t){single_tap, single_hold, double_tap, double_tap, TDM_KC, single_hold_action, double_tap_action, double_tap_action}), }

#define ACTION_TAP_DANCE_QUAD(single_tap, single_hold, single_hold_action, double_tap, double_tap_action, double_hold, double_hold_action) \
    { .fn = {NULL, tap_dance_quad_finished, tap_dance_quad_reset, NULL}, \
	  .user_data = (void *)&((tap_dance_quad_t){single_tap, single_hold, double_tap, double_hold, TDM_KC, single_hold_action, double_tap_action, double_hold_action}), }
	  
#define ACTION_TAP_DANCE_QUAD_FULL(single_tap, single_tap_action, single_hold, single_hold_action, double_tap, double_tap_action, double_hold, double_hold_action) \
    { .fn = {NULL, tap_dance_quad_finished, tap_dance_quad_reset, NULL}, \
	  .user_data = (void *)&((tap_dance_quad_t){single_tap, single_hold, double_tap, double_hold, single_tap_action, single_hold_action, double_tap_action, double_hold_action}), }
	  
void tap_dance_tap_hold_on_each_release(tap_dance_state_t *state, void *user_data);
void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data);
void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data);

void tap_dance_quad_finished(tap_dance_state_t *state, void *user_data);
void tap_dance_quad_reset(tap_dance_state_t *state, void *user_data);
