/* keymap.h
 *
 * A couple things have been moved here to keep keymap.c clean.
 *
 * Author: Ryan Turner
 */

#pragma once

// These may or may not already be defined by QMK dependencies
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef SIGN
#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#endif

// Maximum length of the OLED line buffer
#define SHOW_LEN 168

// Fixed point integer divisor
#define FP_DIV 1000
#define WPM_EMA_ALPHA 200 // Alpha is stored in fixed point
#define WPM_DIV 20 // Conversion ratio where velocity = wpm / WPM_DIV


int closestMultiple(int n, int m);

// Used in layer_info to assign names to layers so they can be shown on the OLED
typedef struct {
    const char* name;
    bool prefix;
} layer_info_t;

// Used in macro_info to define macro keycodes and associate them with a name to be shown on the OLED
typedef struct {
	const char* type;
	const char* name;
	const char* macro;
} macro_info_t;

#define MACRO(type, name, send_string) {type, name, send_string}

// Macros to convert from macro_keycode to macro_info index
#define M_INDEX(kc) kc - (MACRO_RANGE_START + 1)
#define M(kc) M_INDEX(kc)

// Enable or disable the OLED display
bool oled_enabled = true;

// Helper functions to streamline oled_task_user 
void show_macro(const char* type, const char* name);
void show_feature(const char* type, const char* name);

int snprintf_append(char *buffer, size_t bufferSize, const char *format, ...);
uint16_t scale_value_lim(uint16_t value, uint16_t min_in, uint16_t max_in, uint16_t min_out, uint16_t max_out);
