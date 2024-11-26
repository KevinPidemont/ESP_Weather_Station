#ifndef FONT
#define FONT

#include <stdint.h>

#define CHAR_0 0
#define CHAR_1 1
#define CHAR_2 2
#define CHAR_3 3
#define CHAR_4 4
#define CHAR_5 5
#define CHAR_6 6
#define CHAR_7 7
#define CHAR_8 8
#define CHAR_9 9
#define CHAR_C 10
#define CHAR_DEGREE 11
#define CHAR_COLON 12

/**
 * @brief Associate one of the predefined char key to their bitmap representation.
 * 
 * This variable holds the bitmaps of all characters that can be drawn.
 * Each character is 5 pixels wide and 6 pixels tall (the last 2 rows are always empty meaning their bits are equals to '0').
 * 
 * @note It is recommended to use predefined constants (e.g., `CHAR_0`, `CHAR_1`, ...) 
 *       to access character bitmaps instead of directly using the array index.
 */
const uint8_t font5x8[][6] = {
    [CHAR_0] = {0x3E, 0x51, 0x49, 0x45, 0x3E},
    [CHAR_1] = {0x00, 0x42, 0x7F, 0x40, 0x00},
    [CHAR_2] = {0x62, 0x51, 0x49, 0x49, 0x46},
    [CHAR_3] = {0x22, 0x41, 0x49, 0x49, 0x36},
    [CHAR_4] = {0x18, 0x14, 0x12, 0x7F, 0x10},
    [CHAR_5] = {0x27, 0x45, 0x45, 0x45, 0x39},
    [CHAR_6] = {0x3E, 0x49, 0x49, 0x49, 0x30},
    [CHAR_7] = {0x01, 0x71, 0x09, 0x05, 0x03},
    [CHAR_8] = {0x36, 0x49, 0x49, 0x49, 0x36},
    [CHAR_9] = {0x06, 0x49, 0x49, 0x29, 0x1E},
    [CHAR_C] = {0x3E, 0x41, 0x41, 0x41, 0x22},
    [CHAR_DEGREE] = {0x00, 0x06, 0x09, 0x09, 0x06},    
    [CHAR_COLON] = {0x00, 0x36, 0x36, 0x00, 0x00},
};

#endif