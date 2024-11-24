#include "ssd1331.h"

#include <stdio.h>
#include <driver/spi_master.h>

#define SSD1331_CMD_DRAWLINE        0x21 //!< Draw line
#define SSD1331_CMD_DRAWRECT        0x22 //!< Draw rectangle
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

    send_command(0x26);
    send_command(0x1);

    /*
    send_command(0x22);

    send_command(0); // Column start
    send_command(0); // Row start
    send_command(95); // Column end
    send_command(63); // Row end

    // Blue color
    send_command(28); // Outline C color
    send_command(28); // Outline B color
    send_command(28); // Outline A color

    
    // Red color
    send_command(28); // Fill area C color
    send_command(0); // Fill area B color
    send_command(0); // Fill area A color

    draw_pixel(20, 20, 0xAAAA);*/
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

}