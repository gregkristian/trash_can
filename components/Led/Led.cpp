#include <stdio.h>
#include "Led.h"

Led::Led(gpio_num_t _pin) {
    pin = _pin;
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, false);
}

void Led::turnOn() {
    gpio_set_level(pin, true);
}

void Led::turnOff() {
    gpio_set_level(pin, false);
}