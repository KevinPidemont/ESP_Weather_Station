#ifndef SSD1331_MODULE
#define SSD1331_MODULE

#include <stdint.h>
#include <driver/gpio.h>

/**
 * @struct ssd1331_desc
 * @brief Descriptor of SSD1331.
 * 
 * This struct holds any information about the SPI pins to use to communicate with the SSD1331.
 * 
 * @note Before doing any operation with the SSD1331, you must ensure that all properties are correctly initialized.
 */
typedef struct
{
    uint8_t mosi; /** Master Out Slave In (MOSI) pin for SPI communication */
    uint8_t dc; /** Data/Command (DC) pin to switch between command and data mode */
    uint8_t cs; /** Chip Select (CS) pin for SPI communication */
    uint8_t rs; /** Reset (RS) pin to hardware reset the SSD1331 */
    uint8_t clk; /** Clock (CLK) pin for SPI communication */
    int spi_clk_speed; /** Set the rate of the clock for SPI communication. Express in Hertz (Hz) */
} ssd1331_desc;

/**
 * @brief Initialize the SPI communication with the SSD1331.
 * 
 * This function configures the SPI interface and prepares the SSD1331 OLED display
 * for communication. It sets up the GPIO pins specified in the `ssd1331_desc` structure 
 * and ensures that the display is ready to accept commands and data.
 * 
 * @param desc A pointer to ssd1331_desc that describes how to initialize SPI communication.
 * 
 * @return
 * - `ESP_OK` if the initialization was successful.
 * - `ESP_ERR_INVALID_ARG` if the `desc` pointer is NULL or if the configuration parameters 
 *   are invalid.
 * - `ESP_FAIL` if the SPI communication initialization fails for other reasons 
 *   (e.g., hardware issues or misconfiguration).
 * 
 * @note 
 * - Ensure that the GPIO pins specified in `desc` are not used by other peripherals.
 * - This function should be called before sending any commands or data to the SSD1331.
 *
 * @warning 
 * - Incorrect or incomplete initialization of the `ssd1331_desc` structure may lead 
 *   to undefined behavior or failure to communicate with the display.
 * - Ensure the power supply to the SSD1331 module is stable before calling this function.
 */
esp_err_t ssd1331_begin(ssd1331_desc* desc);

/**
 * @brief Turn on the screen.
 * 
 * @note This method simply turns on the screen, so any previous content drawn may still appear.
 *       If you want a clean screen, consider calling `ssd1331_clear_window()` before using this method.
 */
void ssd1331_turn_on(void);

/**
 * @brief Turn off the scren.
 */
void ssd1331_turn_off(void);

/**
 * @brief Clear the entire screen's content.
 * 
 * This function clears all content displayed on the SSD1331 screen by filling it 
 * with a default background color, typically black.
 * 
 * @note This function does not turn off the screen. Use `ssd1331_turn_off()` if 
 *       you want to power down the display after clearing it.
 */
void ssd1331_clear_window(void);
#endif