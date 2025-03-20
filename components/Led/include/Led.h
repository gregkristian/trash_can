#include "driver/gpio.h"

class Led
{
private:
    gpio_num_t pin;

public:
    Led(gpio_num_t _pin);
    
    void turnOn();
    void turnOff();
};