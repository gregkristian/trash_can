
#include "UltrasonicSensor.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "Sensor";
#define PULSE_TIMEOUT 1000000  // 1s

UltrasonicSensor::UltrasonicSensor(gpio_num_t _echoPin, gpio_num_t _triggerPin) {
    echoPin = _echoPin;
    triggerPin = _triggerPin;
    gpio_reset_pin(echoPin);
    gpio_reset_pin(triggerPin);
    gpio_set_direction(echoPin, GPIO_MODE_INPUT);
    gpio_set_direction(triggerPin, GPIO_MODE_OUTPUT);
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

long UltrasonicSensor::measureDistance() {
    // Send TRIGGER signal
    gpio_set_level(triggerPin, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);  
    gpio_set_level(triggerPin, true);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(triggerPin, false);
    vTaskDelay(2 / portTICK_PERIOD_MS);

    // Measure the ECHO signal
    long duration = pulseIn(echoPin, 1, PULSE_TIMEOUT);
    long distance_in_cm = convertMicrosecToCm(duration);
    esp_rom_delay_us(25000);  
    return distance_in_cm;
}

long UltrasonicSensor::convertMicrosecToCm(long duration) {
    /**
	* Do the measurement calculation and return result in centimeter
	* Sound travels with 340m/sec speed.
	* Example: Obstace 100cm away from sensor. Measure time is 100cm to
	* obstacle and 100cm return = 200cm
	* 
	* 1sec = 1000ms = 1.000.000uS
	* 1.000.000 / 340 = Distance in microseconds for 100cm
	* 2941uS fuer 100cm = 5882 uS fuer 200cm
	*
	* duration / 5882 * 100 = distance in cm
	*/	
    long distance_in_cm = (duration * 100) / 5882;

    return distance_in_cm;
}