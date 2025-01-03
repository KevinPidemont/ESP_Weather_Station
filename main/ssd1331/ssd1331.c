#include "ssd1331.h"

#include <stdio.h>
#include <driver/spi_master.h>
#include "font.h"

#define SSD1331_CMD_DRAWLINE        0x21 //!< Draw line
#define SSD1331_CMD_DRAWRECT        0x22 //!< Draw rectangle
#define SSD1331_CMD_CLEARWINDOW     0x25 //!< Clear window
#define SSD1331_CMD_FILL            0x26 //!< Fill enable/disable
#define SSD1331_CMD_SETCOLUMN       0x15 //!< Set column address
#define SSD1331_CMD_SETROW          0x75 //!< Set row adress
#define SSD1331_CMD_CONTRASTA       0x81 //!< Set contrast for color A
#define SSD1331_CMD_CONTRASTB       0x82 //!< Set contrast for color B
#define SSD1331_CMD_CONTRASTC       0x83 //!< Set contrast for color C
#define SSD1331_CMD_MASTERCURRENT   0x87 //!< Master current control
#define SSD1331_CMD_SETREMAP        0xA0 //!< Set re-map & data format
#define SSD1331_CMD_STARTLINE       0xA1 //!< Set display start line
#define SSD1331_CMD_DISPLAYOFFSET   0xA2 //!< Set display offset
#define SSD1331_CMD_NORMALDISPLAY   0xA4 //!< Set display to normal mode
#define SSD1331_CMD_DISPLAYALLON    0xA5 //!< Set entire display ON
#define SSD1331_CMD_DISPLAYALLOFF   0xA6 //!< Set entire display OFF
#define SSD1331_CMD_INVERTDISPLAY   0xA7 //!< Invert display
#define SSD1331_CMD_SETMULTIPLEX    0xA8 //!< Set multiplex ratio
#define SSD1331_CMD_SETMASTER       0xAD //!< Set master configuration
#define SSD1331_CMD_DISPLAYOFF      0xAE //!< Display OFF (sleep mode)
#define SSD1331_CMD_DISPLAYON       0xAF //!< Normal Brightness Display ON
#define SSD1331_CMD_POWERMODE       0xB0 //!< Power save mode
#define SSD1331_CMD_PRECHARGE       0xB1 //!< Phase 1 and 2 period adjustment
#define SSD1331_CMD_CLOCKDIV        0xB3 //!< Set display clock divide ratio/oscillator frequency
#define SSD1331_CMD_PRECHARGEA      0x8A //!< Set second pre-charge speed for color A
#define SSD1331_CMD_PRECHARGEB      0x8B //!< Set second pre-charge speed for color B
#define SSD1331_CMD_PRECHARGEC      0x8C //!< Set second pre-charge speed for color C
#define SSD1331_CMD_PRECHARGELEVEL  0xBB //!< Set pre-charge voltage
#define SSD1331_CMD_VCOMH           0xBE //!< Set Vcomh voltge

/**
 * Internal variable that holds the SPI instance.
 */
spi_device_handle_t spi;

/**
 * Internally hold a reference to the descriptor.
 */
ssd1331_desc global_desc;

/**
 * Internal method that send a command to the SSD1331.
 */
esp_err_t send_command(uint8_t command) {
    gpio_set_level(global_desc.dc, 0); // Set the Data/Command pin to act as command.

    spi_transaction_t t = {
        .length = 8, // 1 byte length command
        .tx_buffer = &command,
    };
    return spi_device_transmit(spi, &t);
}

/**
 * Internal method that send a command to the SSD1331.
 */
esp_err_t send_data(uint8_t data) {
    gpio_set_level(global_desc.dc, 1); // Set the Data/Command pin to act as data.

    spi_transaction_t t = {
        .length = 8, // 1 byte length command
        .tx_buffer = &data,
    };
    return spi_device_transmit(spi, &t);
}

void draw_pixel(uint8_t x, uint8_t y, uint16_t color) {
    send_command(SSD1331_CMD_SETCOLUMN);
    send_command(x);
    send_command(x);

    send_command(SSD1331_CMD_SETROW);
    send_command(y);
    send_command(y);

    send_data(color >> 8);
    send_data(color & 0xFF);
}

void default_loading_sequence(void) {
    // This loading sequence come from the Adafruit library
    send_command(SSD1331_CMD_DISPLAYOFF); // 0xAE
    send_command(SSD1331_CMD_SETREMAP);   // 0xA0
    send_command(0x72); // RGB Color
    send_command(SSD1331_CMD_STARTLINE); // 0xA1
    send_command(0x0);
    send_command(SSD1331_CMD_DISPLAYOFFSET); // 0xA2
    send_command(0x0);
    send_command(SSD1331_CMD_NORMALDISPLAY); // 0xA4
    send_command(SSD1331_CMD_SETMULTIPLEX);  // 0xA8
    send_command(0x3F);                      // 0x3F 1/64 duty
    send_command(SSD1331_CMD_SETMASTER);     // 0xAD
    send_command(0x8E);
    send_command(SSD1331_CMD_POWERMODE); // 0xB0
    send_command(0x0B);
    send_command(SSD1331_CMD_PRECHARGE); // 0xB1
    send_command(0x31);
    send_command(SSD1331_CMD_CLOCKDIV); // 0xB3
    send_command(0xF0); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio
                        // (A[3:0]+1 = 1..16)
    send_command(SSD1331_CMD_PRECHARGEA); // 0x8A
    send_command(0x64);
    send_command(SSD1331_CMD_PRECHARGEB); // 0x8B
    send_command(0x78);
    send_command(SSD1331_CMD_PRECHARGEC); // 0x8C
    send_command(0x64);
    send_command(SSD1331_CMD_PRECHARGELEVEL); // 0xBB
    send_command(0x3A);
    send_command(SSD1331_CMD_VCOMH); // 0xBE
    send_command(0x3E);
    send_command(SSD1331_CMD_MASTERCURRENT); // 0x87
    send_command(0x06);
    send_command(SSD1331_CMD_CONTRASTA); // 0x81
    send_command(0x91);
    send_command(SSD1331_CMD_CONTRASTB); // 0x82
    send_command(0x50);
    send_command(SSD1331_CMD_CONTRASTC); // 0x83
    send_command(0x7D);
    send_command(SSD1331_CMD_DISPLAYON); //--turn on oled panel

    send_command(SSD1331_CMD_FILL);
    send_command(0x1);

    ssd1331_clear_window();
}

esp_err_t ssd1331_begin(ssd1331_desc* desc) {
    if (desc == NULL) {
        printf("ssd1331_begin: desc argument is NULL!");
        return ESP_ERR_INVALID_ARG;
    }

    global_desc = *desc;

    // Configure RS (Reset) and D/C (Data/Command) pin as output
    gpio_set_direction(desc->rs, GPIO_MODE_OUTPUT);
    gpio_set_direction(desc->dc, GPIO_MODE_OUTPUT);
    
    // Start by hardware resetting the SSD1331
    gpio_set_level(desc->rs, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(desc->rs, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .miso_io_num = -1, // No MISO for SSD1331
        .mosi_io_num = desc->mosi,
        .sclk_io_num = desc->clk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    // SPI device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = desc->spi_clk_speed,
        .mode = 0,
        .spics_io_num = desc->cs,
        .queue_size = 7,
        .pre_cb = NULL
    };
    
    // Initialize SPI bus
    esp_err_t result = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);

    if (result != ESP_OK) {
        return result;
    }

    // Add the device onto the SPI bus
    result = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);

    if (result != ESP_OK) {
        // Try to clean up in case of failure
        spi_bus_free(HSPI_HOST);
        return result;
    }

    default_loading_sequence();

    return ESP_OK;
}

void ssd1331_turn_on(void) {
    send_command(SSD1331_CMD_DISPLAYON);
}

void ssd1331_turn_off(void) {
    send_command(SSD1331_CMD_DISPLAYOFF);
}

void ssd1331_clear_window(void) {
    send_command(SSD1331_CMD_CLEARWINDOW);
    send_command(0);
    send_command(0);
    send_command(SSD1331_WIDTH - 1);
    send_command(SSD1331_HEIGHT - 1);
}

void ssd1331_draw_char(uint8_t x, uint8_t y, uint8_t char_id, uint16_t color) {
    // When the given char_id is provided, we abort this operation
    if (char_id >= 15) {
        return;
    }

    if (x >= SSD1331_WIDTH) {
        x = SSD1331_WIDTH - 1;
    }

    if (y >= SSD1331_HEIGHT) {
        y = SSD1331_HEIGHT - 1;
    }

    // Get the character bitmap
    const uint8_t *bitmap = font5x8[char_id];

   
    // Iterate over the bitmap columns.
    for (int i = 0; i < 5; i++) {
        // Get the current column representation.
        uint8_t line = bitmap[i];

        // Every character are defined using a byte.
        // As the character are 7-pixels tall, we don't compute the empty line.
        for (int j = 1; j < 8; j++) {
            // Only draw the pixel when the current bit is high.
            if (line & 0x01) {
                draw_pixel(x + i, y + j, color);
            }

            // Shift to the next bit to compute.
            line >>= 1;
        }
    }
}

// Internal method that map a char to one of our custom char id
int8_t map_char_to_font_id(char c) {
    printf("[map_char_to_font_id] c=%c, d=%d\n", c, c);
    switch (c) {
        case '0': return CHAR_0;
        case '1' : return CHAR_1;
        case '2' : return CHAR_2;
        case '3' : return CHAR_3;
        case '4' : return CHAR_4;
        case '5' : return CHAR_5;
        case '6' : return CHAR_6;
        case '7' : return CHAR_7;
        case '8' : return CHAR_8;
        case '9' : return CHAR_9;
        case 'C' : return CHAR_C;
        case ':' : return CHAR_COLON;
        case '°' : return CHAR_DEGREE;
        case '.' : return CHAR_DOT;
        case '%' : return CHAR_PERCENT;
        default: return -1;
    }
}

void ssd1331_draw_string(uint8_t x, uint8_t y, const char *text, uint16_t color) {
    uint8_t x_offset = x;

    while (*text) {
        const int8_t char_id = map_char_to_font_id(*text);
        text++;
        if (char_id < 0) {
            continue;
        }

        ssd1331_draw_char(x_offset, y, char_id, color);
        x_offset += 7; // Each character are 6 pixels wide, we add a space of 1 pixel between each character
    }
}