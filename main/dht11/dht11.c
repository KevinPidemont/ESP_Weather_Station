#include "dht11.h"
#include <dht.h>

esp_err_t read_dht_11(gpio_num_t pin, float *humidity, float *temperature) {
    return dht_read_float_data(DHT_TYPE_DHT11, pin, humidity, temperature);
}