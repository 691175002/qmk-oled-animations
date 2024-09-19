/* dynamic_macro_status.h
 *
 * Implements several dynamic macro callbacks to track the state and size of both macro slots.
 * Declares enums for macro states, variables for macro storage, and prototypes for macro management functions.
 *
 * Author: Ryan Turner
 */
#pragma once

#include "quantum.h"

typedef enum {
	DM_EMPTY,
	DM_RECORDING,
	DM_SET,
} dm_state_t;

// State of each macro
dm_state_t dm_state_1;
dm_state_t dm_state_2;

// Length of each macro, after recording
uint16_t dm_size_1;
uint16_t dm_size_2;

void dynamic_macro_record_start_user(int8_t direction);
void dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record);
void dynamic_macro_record_end_user(int8_t direction);
