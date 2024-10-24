#ifndef BUTTONS
#define BUTTONS

#include "main.h"

#define SHORT_PRESS_TIME        250
#define LONG_PRESS_TIME         2000
#define EXTRA_LONG_PRESS_TIME   4000

//typedef struct {
//    GPIO_TypeDef* port;  // Pointer to the GPIO port (e.g., GPIOA, GPIOB)
//    uint16_t pin;        // GPIO pin number (e.g., GPIO_PIN_0, GPIO_PIN_1, etc.)
//} GPIO_Pin_t;

class Button
{
    public:
        // Button(const Button&) = delete;
        // Button& operator=(const Button&) = delete;
        // static Button* getInstance(uint8_t pin, unsigned long time);
        Button(GPIO_Pin_t pin)
        : _pin(pin), _debounce_time(DEBOUNCE_TIME_MS), _last_debounce_time(0), _state(RESET), _last_button_state(RESET), _button_press_time(0)
        {   }

        void update();
        BUTTON_STATE_e getPressDuration();
    private:

        GPIO_Pin_t _pin;
        unsigned long  _debounce_time, _last_debounce_time;
        bool    _state, _last_button_state;
        uint64_t    _button_press_time;
        BUTTON_STATE_e _press_duration;

};


#endif
