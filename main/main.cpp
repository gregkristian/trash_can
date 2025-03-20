/* TRASH CAN
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "UltrasonicSensor.h"
#include "Servo.h"

#define DELAY_MS        500  // Delay in main loop
#define DISTANCE_SENSOR 10   // Distance in cm to open the lock
#define TIMEOUT_LOCK    2000 // Open for 2 seconds then close if sensor doesn't detect anything
#define LED_PIN         (gpio_num_t)4    // GPIO pin for LED

static const char *TAG = "Main";

extern "C" void app_main(void)
{ 
    ESP_LOGI(TAG, "Starting main");

    UltrasonicSensor UltrasonicSensor((gpio_num_t)23, (gpio_num_t)20);
    Servo servo;
    servo.servo_init();
    servo.servo_set_angle(160);
    vTaskDelay(pdMS_TO_TICKS(1000));

    int timer = TIMEOUT_LOCK;
    bool lock_is_open = false;

    // Init LED
    gpio_reset_pin(LED_PIN); 
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); 
    gpio_set_level(LED_PIN, false);  // Turn off LED
    
    while(1) {
        long dist = UltrasonicSensor.measureDistance();
        vTaskDelay(pdMS_TO_TICKS(200));

        // Open the lock if object detected
        if (dist < DISTANCE_SENSOR && lock_is_open == false) {
            ESP_LOGI(TAG, "Object detected, distance %ld cm. OPENING LOCK", dist);
            servo.servo_set_angle(0);
            gpio_set_level(LED_PIN, true);  // Turn on LED
            lock_is_open = true;
        }
        
        // Object no longer detected. Start countdown to close the lock, then close it when timout expires
        if (dist >= DISTANCE_SENSOR && lock_is_open == true) {
            ESP_LOGI(TAG, "Object no longer detected, counting down to close the lock");
            timer -= DELAY_MS;

            // Timer expired, close the lock
            if (timer <= 0) {
                ESP_LOGI(TAG, "Timeout. CLOSING LOCK");
                servo.servo_set_angle(160);
                gpio_set_level(LED_PIN, false);  // Turn off LED
                lock_is_open = false;
                timer = TIMEOUT_LOCK;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}