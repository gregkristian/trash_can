/* TRASH CAN
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "SR04.h" // Update the path to the correct location of SR04.h

static const char *TAG = "Main";


extern "C" void app_main(void)
{ 
    ESP_LOGI(TAG, "Starting main");

    SR04 sr04((gpio_num_t)23, (gpio_num_t)20);
    
    ESP_LOGI(TAG, "Ending main");

    while(1) {
        long dist = sr04.Distance();
        ESP_LOGI(TAG, "Distance %ld cm", dist);
        if (dist < 10) {
            ESP_LOGI(TAG, "OPENING LOCK");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
