/* dynamic_macro_status.c
 *
 * Implements several dynamic macro callbacks to track the state and size of both macro slots.
 *
 * Author: Ryan Turner
 */

#include "dynamic_macro_status.h"

uint16_t dm_keycount;

dm_state_t dm_state_1 = DM_EMPTY;
dm_state_t dm_state_2 = DM_EMPTY;

void dynamic_macro_record_start_user(int8_t direction) {
	dm_keycount = 0;
	
	if (direction == 1) {
		dm_state_1 = DM_RECORDING;
	} else if (direction == -1) {
		dm_state_2 = DM_RECORDING;
	}
}

void dynamic_macro_record_key_user(int8_t direction, keyrecord_t *record)  {
	dm_keycount += 1;
}

void dynamic_macro_record_end_user(int8_t direction) {
	// Note: DM_RSTP is counted if it is on a layer hence the -1
	if (direction == 1) {
		dm_state_1 = (dm_keycount > 1) ? DM_SET : DM_EMPTY;
		dm_size_1 = (dm_keycount - 1) / 2;
	} else if (direction == -1) {
		dm_state_2 = (dm_keycount > 1) ? DM_SET : DM_EMPTY;
		dm_size_2 = (dm_keycount - 1) / 2;
	}
}
