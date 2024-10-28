#ifndef BUTTONS
#define BUTTONS

#include "main.h"

#define SHORT_PRESS_TIME        250
// #define LONG_PRESS_TIME         2000
// #define EXTRA_LONG_PRESS_TIME   4000


class Button
{
    public:
        // Button(const Button&) = delete;
        // Button& operator=(const Button&) = delete;
        // static Button* getInstance(uint8_t pin, unsigned long time);
        Button(GPIO_Pin_t pin, GPIO_Pin_t led)
        : _pin(pin), _led_pin(led), _debounce_time(DEBOUNCE_TIME_MS), _last_debounce_time(0), _state(GPIO_PIN_RESET), _last_button_state(GPIO_PIN_RESET), _button_press_time(0)
        {   HAL_GPIO_WritePin(_led_pin.port, _led_pin.pin, GPIO_PIN_RESET);     }

        void update();
        GPIO_PinState getButtonState();
    private:

        GPIO_Pin_t     _pin, _led_pin;
        unsigned long  _debounce_time, _last_debounce_time;
        GPIO_PinState  _state, _last_button_state;
        uint64_t       _button_press_time;

};


#endif
