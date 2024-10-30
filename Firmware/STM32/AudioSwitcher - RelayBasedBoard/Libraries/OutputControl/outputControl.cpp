#include "outputControl.hpp"

#define TO_MS(x)	(x*1000)
#define OUTPUT_ACTIVE_TIME	TO_MS(30)

void OutputControl::process()
{
	_sw->update();
	if((GPIO_PIN_RESET != _sw->getButtonState()) && (GPIO_PIN_SET != _state))
	{
		_output->start();
		_on_time = HAL_GetTick();
		_state = GPIO_PIN_SET;
	}

	if(((HAL_GetTick()-_on_time) > OUTPUT_ACTIVE_TIME) && (GPIO_PIN_RESET != _state))
	{
		_output->stop();
		_state = GPIO_PIN_RESET;
	}
}

bool OutputControl::getState()
{
	return _state;
}
