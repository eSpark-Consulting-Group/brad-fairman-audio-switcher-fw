#include "button.hpp"


void Button::update()
{
    bool reading = HAL_GPIO_ReadPin(_pin.port, _pin.pin);
        
    if (reading != _last_button_state)
    {
        _last_debounce_time = HAL_GetTick();
    }

    if ((HAL_GetTick() - _last_debounce_time) > _debounce_time)
    {
        if (reading != _state)
        {
            _state = reading;

            if (_state == RESET)
            {
                _button_press_time = HAL_GetTick();
            }
            else
            {
                unsigned long pressDurationTime = HAL_GetTick() - _button_press_time;
                if (pressDurationTime < SHORT_PRESS_TIME) {
                    _press_duration = SHORT_PRESS;
                } else {
                    _press_duration = RELEASED;
                }
            }
        }
    }

    if (_state == RESET)
    {
        unsigned long currentDuration = HAL_GetTick() - _button_press_time;
        if (currentDuration >= EXTRA_LONG_PRESS_TIME)
        {
            _press_duration = EXTRA_LONG_PRESS;
        } else if (currentDuration >= LONG_PRESS_TIME)
        {
            _press_duration = LONG_PRESS;
        }
    }

    _last_button_state = reading;
}

BUTTON_STATE_e Button::getPressDuration()
{
    if (_state == SET)
    {
        return _press_duration; // Return last detected press duration when released
    }
    return RELEASED; // If still pressed, return RELEASED state
}
