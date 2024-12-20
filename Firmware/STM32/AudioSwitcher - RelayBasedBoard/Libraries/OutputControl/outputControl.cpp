#include "outputControl.hpp"

#define SEC_TO_MS(x)	(x*1000)
#define OUTPUT_ACTIVE_TIME	SEC_TO_MS(30)

#define GPIO_OUT1_TOGGLE_TIME	250

void OutputControl::activate()
{
	for(int i=0; i<_active_outputs; i++)
	{
		if((_output.output[i]) && (!_output.output[i]->isActive()))
		{
			_output.output[i]->start();
		}
	}
	HAL_GPIO_WritePin(_gpio_out_1.port, _gpio_out_1.pin, GPIO_PIN_RESET);
	_on_time = HAL_GetTick();
	_gpio_out_1_time = HAL_GetTick();
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
	if(((HAL_GetTick()-_gpio_out_1_time) > GPIO_OUT1_TOGGLE_TIME) && (GPIO_PIN_SET != HAL_GPIO_ReadPin(_gpio_out_1.port, _gpio_out_1.pin)))
	{
		HAL_GPIO_WritePin(_gpio_out_1.port, _gpio_out_1.pin, GPIO_PIN_SET);
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
		futureMode = currentMode + 4;
	}
	else if((buttonNumber == OUT4_SW) && (currentMode < ALL_OFF_MODE))
	{
		futureMode = currentMode - 4;
	}
	else if (buttonNumber < OUT4_SW){
		if((GPIO_PIN_SET == getState()) && !(buttonNumber ^ (currentMode&0x3)))
			futureMode = ALL_OFF_MODE;
		else
			futureMode = buttonNumber;
	}
	else if (buttonNumber == OUT5_SW)
	{
		if((GPIO_PIN_SET == getState()) && (ALL_ACTIVE_MODE == currentMode))
			futureMode = ALL_OFF_MODE;
		else
			futureMode = ALL_ACTIVE_MODE;
	}

	return futureMode;
}

void OutputControl::addOutput(Interface* output)
{
  for(int i=0; i < (NUM_OF_OUTPUTS+1); i++)
  {
	  if(_output.output[i] == output)
	  {
		  return;
	  }
  }
	_output.output[_active_outputs++] = output;
	_state = GPIO_PIN_RESET;
}

void OutputControl::removeOutput(Interface* output)
{
  for(int i=0; i < (NUM_OF_OUTPUTS+1); i++)
  {
	  if(_output.output[i] == output)
	  {
		  _output.output[i]->stop();
		  for(int y = i; y < (NUM_OF_OUTPUTS+1); y++)
		  	  _output.output[y] = ((y+1) < (NUM_OF_OUTPUTS+1)) ? _output.output[y+1] : NULL;
		  _active_outputs--;
		  break;
	  }
  }
}

void OutputControl::removeAllOutputs()
{
  for(int i=0; i < (NUM_OF_OUTPUTS+1); i++)
  {
	  if(_output.output[i])
	  {
		  _output.output[i]->stop();
		  _output.output[i] = NULL;
	  }
  }
  _active_outputs = 0;
  _state = GPIO_PIN_RESET;
}

void OutputControl::turnOffAllOutputs()
{
  for(int i=0; i < (NUM_OF_OUTPUTS+1); i++)
  {
	  if(_output.output[i] && _output.output[i]->isActive())
	  {
		  _output.output[i]->stop();
	  }
  }
}
