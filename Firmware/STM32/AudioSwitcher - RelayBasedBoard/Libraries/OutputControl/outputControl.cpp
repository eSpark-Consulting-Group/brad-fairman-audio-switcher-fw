#include "outputControl.hpp"

#define TO_MS(x)	(x*1000)
#define OUTPUT_ACTIVE_TIME	TO_MS(30)

void OutputControl::activate()
{
	for(int i=0; i<_active_outputs; i++)
	{
		if((_output.output[i]) && (!_output.output[i]->isActive()))
		{
			_output.output[i]->start();
		}
	}
	_on_time = HAL_GetTick();
	_state = GPIO_PIN_SET;
}

void OutputControl::process()
{
	if(((HAL_GetTick()-_on_time) > OUTPUT_ACTIVE_TIME) && (GPIO_PIN_RESET != getState()))
	{
		turnOffAllOutputs();
		_state = GPIO_PIN_RESET;
		*_mode = ALL_OFF_MODE;
	}
}

bool OutputControl::getState()
{
	return _state;
}

uint8_t OutputControl::getMode(uint8_t buttonNumber)
{
	uint8_t currentMode = (uint8_t)*_mode;
	uint8_t futureMode = currentMode;

	if((buttonNumber == OUT4_SW) && (currentMode < SUBWOOFER_MODE))
	{
		futureMode = 4 + currentMode;
	}
	else if((buttonNumber == OUT4_SW) && (currentMode < ALL_ACTIVE_MODE))
	{
		futureMode = NO_CHANGE;
	}
	else if (buttonNumber < OUT4_SW){
		futureMode = buttonNumber;
	}
	else if (buttonNumber == OUT5_SW)
	{
		futureMode = ALL_ACTIVE_MODE;
	}

	return futureMode;
}

void OutputControl::addOutput(Interface* output)
{
  for(int i=0; i < NUM_OF_OUTPUTS+1; i++)
  {
	  if(_output.output[i] == output)
	  {
		  return;
	  }
  }
	_output.output[_active_outputs++] = output;
	_state = GPIO_PIN_RESET;
}

void OutputControl::removeAllOutputs()
{
  for(int i=0; i < NUM_OF_OUTPUTS+1; i++)
  {
	  if(_output.output[i])
	  {
		  _output.output[i]->stop();
		  _output.output[i] = NULL;
	  }
  }
  _active_outputs = 0;
}

void OutputControl::turnOffAllOutputs()
{
  for(int i=0; i < NUM_OF_OUTPUTS+1; i++)
  {
	  if(_output.output[i] && _output.output[i]->isActive())
	  {
		  _output.output[i]->stop();
	  }
  }
}
