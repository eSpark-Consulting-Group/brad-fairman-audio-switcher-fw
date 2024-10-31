#include "button.hpp"

#define LED_TOGGLE_TIME		500

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
            if(_state == GPIO_PIN_RESET)
            {
            	_trigger = GPIO_PIN_SET;
            	_triggerred = true;
            }
        }
    }
	if (this->isActive() && this->getType())
	{
		if((HAL_GetTick() - _active_Toggle_Time) > LED_TOGGLE_TIME)
		{
			_active_Toggle_Time = HAL_GetTick();
			HAL_GPIO_TogglePin(_led_pin.port, _led_pin.pin);
		}
	}
	else
	{
		HAL_GPIO_WritePin(_led_pin.port, _led_pin.pin, GPIO_PIN_SET);
	}

    _last_button_state = reading;
}

GPIO_PinState Button::getButtonState()
{
//	if(_state == GPIO_PIN_RESET)
//		static bool trigger = true;
//		trigger
//		return GPIO_PIN_SET;
//	else if(_state == GPIO_PIN_SET)
//		return GPIO_PIN_RESET;
//	GPIO_PinState lTrigger = _trigger;
	if(_triggerred && (_trigger == GPIO_PIN_SET))
	{
		_trigger = GPIO_PIN_RESET;
		_triggerred = false;
		return GPIO_PIN_SET;
	}

	return GPIO_PIN_RESET;
}

bool Button::isActive()
{
	return _is_active;
}

void Button::setState(bool state)
{
	_is_active = state;
}

bool Button::getType()
{
	return _is_output_control_button;
}
