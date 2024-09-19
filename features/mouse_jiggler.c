/* mouse_jiggler.c
 *
 * Mostly adapted from https://getreuer.info/posts/keyboards/macros3/index.html
 * Defines a mouse jiggler function to prevent screen savers or sleep mode by simulating mouse movements.
 * Features two modes (fast and slow) to adjust the speed of the jiggling effect.
 *
 * Performs a slow movement by default, #define JIGGLE_FAST to use a faster movement.
 *
 * Requires the following features in rules.mk
 * DEFERRED_EXEC_ENABLE = yes
 * MOUSE_ENABLE = yes
 *
 * Author: Ryan Turner
 */
 
#include "mouse_jiggler.h"
#include "quantum.h"

#ifdef JIGGLE_FAST
	#define DELTAS_SIZE 32
	static const int8_t deltas[32] = {
		0,-1,-2,-2,-3,-3,-4,-4,-4,-4,-3,-3,-2,-2,-1, 0,
		0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0
	};
#else
	#define DELTAS_SIZE 64
	static const int8_t deltas[DELTAS_SIZE] = {
		0, 0,-1,-1,-1,-2,-2,-2,-2,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-2,-2,-2,-2,-1,-1,-1, 0, 0,
		0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 0, 0
	};
#endif

static deferred_token token = INVALID_DEFERRED_TOKEN;
static report_mouse_t report = {0};

void jiggle_start(void) {
	uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
		static uint8_t phase = 0;
		
		report.x = deltas[phase];
		report.y = deltas[(phase + (DELTAS_SIZE / 4)) & (DELTAS_SIZE - 1)];
		
		host_mouse_send(&report);
		
		phase = (phase + 1) & (DELTAS_SIZE - 1);
		
		return 16; // Callback in 16ms/60fps
	}
	
	token = defer_exec(1, jiggler_callback, NULL);
}

void jiggle_stop(void) {
	if (token) {
		cancel_deferred_exec(token);
		token = INVALID_DEFERRED_TOKEN;
		report = (report_mouse_t){}; // Clear mouse
		host_mouse_send(&report);
	}
}

bool jiggle_state(void) {
	return token;
}
