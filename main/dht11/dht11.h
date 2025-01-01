#ifndef DHT11
#define DHT11

#include <stdint.h>
#include <driver/gpio.h>

/**
 * @brief Read the temperature and humidity from the DHT11 sensor.
 * 
 * Humidity and temperature are returned as integers.
 * For example: humidity=625 is 62.5 %, temperature=244 is 24.4 degrees Celsius
 * 
 * @param pin The GPIO connected to the DHT11 OUT.
 * @param[out] humidity The humidity level, may be NULL.
 * @param[out] temperature The temperature in Celsius, may be NULL.
 * 
 * @return The result of the operation, such as :
 * - ESP_OK : When data has successfully been read.
 * - ESP_FAIL : When something went wrong when reading the sensor.
 */
esp_err_t read_dht_11(gpio_num_t pin, float *humidity, float *temperature);

#endif