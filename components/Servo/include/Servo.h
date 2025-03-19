#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PWM_FREQ        5000        // PWM Frequency in Hz
#define PWM_RESOLUTION  LEDC_TIMER_8_BIT  // 8-bit resolution (0-255)
#define PWM_TIMER       LEDC_TIMER_0
#define PWM_CHANNEL     LEDC_CHANNEL_0

#define DUTY_CYCLE_MAX 200
#define DUTY_CYCLE_MIN 0
#define DUTY_CYCLE_STEP 10
#define DUTY_CYCLE_RANGE (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN)

class Servo
{
private:
    ledc_channel_config_t channel_conf = {}; // Config for LED
    ledc_timer_config_t timer_conf = {};  // Config for LED
    
public:
    void servo_init();
    void servo_set_angle(int angle);
};