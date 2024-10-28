#include "button.hpp"


void Button::update()
{
    GPIO_PinState reading = HAL_GPIO_ReadPin(_pin.port, _pin.pin);
        
    if (reading != _last_button_state)
    {
        _last_debounce_time = HAL_GetTick();
    }

    if ((HAL_GetTick() - _last_debounce_time) > _debounce_time)
    {
        if (reading != _state)
        {
            _state = reading;

            if (_state == GPIO_PIN_SET)
            {
                _button_press_time = HAL_GetTick();
                HAL_GPIO_WritePin(_led_pin.port, _led_pin.pin, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(_led_pin.port, _led_pin.pin, GPIO_PIN_SET);
            }
        }
    }

    _last_button_state = reading;
}

GPIO_PinState Button::getButtonState()
{
	if(_state == GPIO_PIN_RESET)
		return GPIO_PIN_SET;
	else if(_state == GPIO_PIN_SET)
		return GPIO_PIN_RESET;
}
