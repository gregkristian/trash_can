/* TRASH CAN
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "UltrasonicSensor.h" // Update the path to the correct location of UltrasonicSensor.h

static const char *TAG = "Main";


extern "C" void app_main(void)
{ 
    ESP_LOGI(TAG, "Starting main");

    UltrasonicSensor UltrasonicSensor((gpio_num_t)23, (gpio_num_t)20);
    
    ESP_LOGI(TAG, "Ending main");

    while(1) {
        long dist = UltrasonicSensor.Distance();
        ESP_LOGI(TAG, "Distance %ld cm", dist);
        if (dist < 10) {
            ESP_LOGI(TAG, "OPENING LOCK");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
