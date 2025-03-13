
#include "SR04.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "Sensor";
#define PULSE_TIMEOUT 1000000  // 1s

SR04::SR04(gpio_num_t echoPin, gpio_num_t triggerPin) {
    _echoPin = echoPin;
    _triggerPin = triggerPin;
    gpio_reset_pin(_echoPin);
    gpio_reset_pin(_triggerPin);
    gpio_set_direction(_echoPin, GPIO_MODE_INPUT);
    gpio_set_direction(_triggerPin, GPIO_MODE_OUTPUT);
    _autoMode = false;
    _distance = 999;
}


long pulseIn(gpio_num_t pin, uint8_t state, int64_t timeout) {
    int64_t start = esp_timer_get_time();

    // Wait for echo pin to get to high state
    while (gpio_get_level(pin) != state) {
        int64_t now = esp_timer_get_time();
        int64_t delta = now - start;
        if (delta > timeout) {
            ESP_LOGI(TAG, "pulseIn %lld %lld | %lld %lld", now, start, delta, timeout);
            return -1;  // Timeout
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);  // Prevent task starvation
    }
    
    // Pulse starting. Record the time
    start = esp_timer_get_time();
    while (gpio_get_level(pin) == state) {
        if (esp_timer_get_time() - start > timeout) {
            return -2;  // Timeout
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);  // Prevent task starvation
    }

    return esp_timer_get_time() - start;
}

long SR04::Distance() {
    long d = 0;
    _duration = 0;
    gpio_set_level(_triggerPin, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);  
    gpio_set_level(_triggerPin, true);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(_triggerPin, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);  
    _duration = pulseIn(_echoPin, 1, PULSE_TIMEOUT);
    //ESP_LOGI(TAG, "_duration %ld", _duration);
    d = MicrosecondsToCentimeter(_duration);
    esp_rom_delay_us(25000);  
    return d;
}

long SR04::MicrosecondsToCentimeter(long duration) {
    long d = (duration * 100) / 5882;
    //d = (d == 0)?999:d;
    return d;
}




