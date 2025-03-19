/* TRASH CAN
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "UltrasonicSensor.h" // Update the path to the correct location of UltrasonicSensor.h
#include "Servo.h"
static const char *TAG = "Main";


extern "C" void app_main(void)
{ 
    ESP_LOGI(TAG, "Starting main");

    UltrasonicSensor UltrasonicSensor((gpio_num_t)23, (gpio_num_t)20);
    Servo servo;
    servo.servo_init();
    servo.servo_set_angle(0);
    vTaskDelay(pdMS_TO_TICKS(3000));
    

    while(1) {
        long dist = UltrasonicSensor.measureDistance();
        ESP_LOGI(TAG, "Distance %ld cm", dist);
        if (dist < 10) {
            ESP_LOGI(TAG, "OPENING LOCK");
            servo.servo_set_angle(90);
        } else {
            servo.servo_set_angle(0);
        }
        
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}