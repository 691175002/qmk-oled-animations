/* bitmaps.h
 *
 * Header file for bitmap rendering on OLED displays in keyboard firmware.
 * Declares the split_render function and provides external references to bitmap data arrays for use across the firmware.
 *
 * Author: Ryan Turner
 */

#pragma once

#include "quantum.h"

void split_render(uint16_t mask, const char *bitmap_full, const char *bitmap_front);
void h_scroll_render(uint16_t offset_x, const char *bitmap, uint16_t bitmap_width);
void v_scroll_render(uint16_t offset_x, const char *bitmap, uint16_t bitmap_width);

extern const char PROGMEM TOTORO_FRONT[];
extern const char PROGMEM TOTORO_FULL[];

extern const char PROGMEM NEKO_FRONT[];
extern const char PROGMEM NEKO_FULL[];

extern const char PROGMEM GHOST_FRONT[];
extern const char PROGMEM GHOST_FULL[];

extern const char PROGMEM WHALE_FRONT[];
extern const char PROGMEM WHALE_FULL[];

extern const char PROGMEM GIRL_ONE[];
extern const char PROGMEM GIRL_TWO[];

extern const char PROGMEM DEMON[];
extern const char PROGMEM DEMON_INV[];

extern const char PROGMEM MAI_FRONT[];
extern const char PROGMEM MAI_FULL[];

# define FACES_WIDTH 768
extern const char PROGMEM FACES[];

# define CAT_WIDTH 360
extern const char PROGMEM CAT_SCROLL[];

# define CHARACTERS_HEIGHT 832
extern const char PROGMEM CHARACTERS[];
