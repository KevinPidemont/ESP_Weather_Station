#include <stdio.h>

#include "ssd1331/ssd1331.h"

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
}
