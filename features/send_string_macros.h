/* send_string_macros.h
 *
 * Defines macros for enhanced string sending and key repetition in keyboard firmware scripts.
 * Simplifies the creation of complex macros, such as opening web pages or repeating key sequences.
 *
 * Author: Ryan Turner
 */


#pragma once

// Open a web address in a new tab
#define SS_WEB(s) SS_LCTL("t") s SS_LCTL("\n")

// Opens a web address after X_WWW_HOME
#define SS_BROWSE(s) SS_TAP(X_WWW_HOME) SS_DELAY(300) SS_LCTL("l") s SS_LCTL("\n")

// Opens a program by searching it in the start menu
# define SS_START(s) SS_TAP(X_LGUI) SS_DELAY(200) s SS_DELAY(200) "\n" SS_DELAY(650)

// Repeat the passed string n times
#define REPT(s, n) REPT_S##n(s)
#define REPT_S1(s) s
#define REPT_S2(s) s s
#define REPT_S3(s) s s s
#define REPT_S4(s) s s s s
#define REPT_S5(s) s s s s s
