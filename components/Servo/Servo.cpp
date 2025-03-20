
#include <stdio.h>
#include "Servo.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "Servo";

#define SERVO_MIN_PULSEWIDTH 1000  // Minimum pulse width in microseconds
#define SERVO_MAX_PULSEWIDTH 2500  // Maximum pulse width in microseconds
#define SERVO_MAX_DEGREE 180       // Maximum angle in degrees
#define SERVO_GPIO_PIN 15          // GPIO pin connected to the servo
#define SERVO_FREQ 50              // Frequency of the PWM signal

void Servo::servo_init()
{
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = SERVO_FREQ,
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
    // First, limit the angle to the range 0 - 180 degrees
    if (angle < 0) {
        angle = 0;
    } else if (angle > SERVO_MAX_DEGREE) {
        angle = SERVO_MAX_DEGREE;
    } 
    
    // Calculate pulse_width. Range is 1 to 2ms, 0 to 180 degrees
    // See http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
    int pulse_width = SERVO_MIN_PULSEWIDTH + ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) / SERVO_MAX_DEGREE;
    
    // Convert pulse_width to LEDC duty cycle (16-bit resolution, 20ms period)
    int duty = (pulse_width * (1 << 16)) / 20000;
    ESP_LOGI(TAG, "angle=%d pulse_width=%d duty=%d ", angle, pulse_width, duty);

    // Execute the duty cycle on pin connected to servo
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}
