#pragma once

#include "main.h"
#include "button.hpp"

class Output
{
	public:
		Output(GPIO_Pin_t relay, GPIO_Pin_t led)
		:	_relay(relay), _indication_LED(led)
		{	this->stop();	}

		void start(void)// override
		{
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_relay.port, _relay.pin, GPIO_PIN_SET);
		}
		void stop(void)// override
		{
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_relay.port, _relay.pin, GPIO_PIN_RESET);
		}
	private:
		GPIO_Pin_t _relay;
		GPIO_Pin_t _indication_LED;
};

class OutputControl
{
	public:
		OutputControl(Button* button, Output* output)
		:	_sw(button), _output(output), _on_time(0), _state(GPIO_PIN_RESET)
		{	}
		void process(void);
		bool getState();
	private:
		Button* _sw;
		Output* _output;
		uint64_t _on_time;
		GPIO_PinState _state;
};
