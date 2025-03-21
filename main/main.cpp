/* TRASH CAN
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Components
#include "UltrasonicSensor.h"
#include "Servo.h"
#include "Led.h"

#define SENSOR_TRIGGER  (gpio_num_t)20   // GPIO pin for sensor trigger
#define SENSOR_ECHO     (gpio_num_t)23   // GPIO pin for sensor echo
#define LED_PIN         (gpio_num_t)4    // GPIO pin for LED
#define SERVO_PIN       (gpio_num_t)15   // GPIO pin for servo


#define DELAY_MS        500  // Delay in main loop
#define DISTANCE_SENSOR 10   // Distance in cm to open the lock
#define TIMEOUT_LOCK    1000 // Open for 2 seconds then close if sensor doesn't detect anything


static const char *TAG = "Main";

extern "C" void app_main(void)
{ 
    ESP_LOGI(TAG, "Starting main");

    // Initialize sensor, servo and LED
    UltrasonicSensor UltrasonicSensor(SENSOR_TRIGGER, SENSOR_ECHO);
    
    Servo servo;
    servo.servo_init();
    servo.servo_set_angle(160);
    
    Led led(LED_PIN);
    led.turnOff();

    int timer = TIMEOUT_LOCK;
    bool lock_is_open = false;

    vTaskDelay(pdMS_TO_TICKS(1000));
    
    while(1) {
        long dist = UltrasonicSensor.measureDistance();
        vTaskDelay(pdMS_TO_TICKS(200));

        // Open the lock if object detected
        if (dist < DISTANCE_SENSOR && lock_is_open == false) {
            ESP_LOGI(TAG, "Object detected, distance %ld cm. OPENING LOCK", dist);
            servo.servo_set_angle(0);
            led.turnOn();  // Turn on LED
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
                led.turnOff();  // Turn off LED
                lock_is_open = false;
                timer = TIMEOUT_LOCK;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}