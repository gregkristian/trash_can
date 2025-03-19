
#include <stdio.h>
#include "Servo.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "Servo";

#define SERVO_MIN_PULSEWIDTH 500   // Minimum pulse width in microseconds
#define SERVO_MAX_PULSEWIDTH 2500  // Maximum pulse width in microseconds
#define SERVO_MAX_DEGREE 180       // Maximum angle in degrees
#define SERVO_GPIO_PIN 15          // GPIO pin connected to the servo

void Servo::servo_init()
{
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 50, // 50Hz PWM frequency
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t channel_conf = {
        .gpio_num = SERVO_GPIO_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_conf);
}

void Servo::servo_set_angle(int angle)
{
    ESP_LOGI(TAG, "set angle %d cm", angle);
    if (angle < 0) angle = 0;
    if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;
    
    uint32_t duty = SERVO_MIN_PULSEWIDTH + ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) / SERVO_MAX_DEGREE;
    duty = (duty * (1 << 16)) / 20000; // Convert to LEDC duty cycle (16-bit resolution, 20ms period)
    ESP_LOGI(TAG, "set angle duty=%lu ", duty);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}
