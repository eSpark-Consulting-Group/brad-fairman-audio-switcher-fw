#include "outputControl.hpp"

#define TO_MS(x)	(x*1000)
#define OUTPUT_ACTIVE_TIME	TO_MS(30)

void OutputControl::process()
{
	_sw->update();
	if((RELEASED != _sw->getPressDuration()) && (true != _state))
	{
		_output->start();
		_on_time = HAL_GetTick();
		_state = true;
	}

	if(((HAL_GetTick()-_on_time) > OUTPUT_ACTIVE_TIME) && (false != _state))
	{
		_output->stop();
		_state = false;
	}
}

bool OutputControl::getState()
{
	return _state;
}
