#include <stdio.h>

#include "ssd1331/ssd1331.h"
#include "ssd1331/font.h"

#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS   5
#define PIN_NUM_DC   4
#define PIN_NUM_RST  2

void app_main(void)
{
    ssd1331_desc desc = {
        .mosi = PIN_NUM_MOSI,
        .clk = PIN_NUM_CLK,
        .cs = PIN_NUM_CS,
        .dc = PIN_NUM_DC,
        .rs = PIN_NUM_RST,
        .spi_clk_speed = 10 * 1000 * 1000, // 10MHz
    };
    ssd1331_begin(&desc);

    // Draw every character on the first row
    uint8_t x = 0;
    for (int i = 0; i < 13; i++) {
        // 7 = 5 pixels wide + 2 padding
        ssd1331_draw_char((7 * i), 0, i, 0xFFFF);
    }
}
