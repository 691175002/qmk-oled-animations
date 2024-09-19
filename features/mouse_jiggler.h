/* mouse_jiggler.h
 *
 * Header file for the mouse jiggler functionality in keyboard firmware.
 * Declares functions for controlling the mouse jiggler, allowing other firmware parts to start or stop jiggling and check its current state.
 *
 * Author: Ryan Turner
 */

#pragma once

#include <stdbool.h>

void jiggle_start(void);
void jiggle_stop(void);
bool jiggle_state(void);
