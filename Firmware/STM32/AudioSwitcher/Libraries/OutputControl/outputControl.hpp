#pragma once

#include "main.h"
#include "MAX14661.h"
#include "button.hpp"


//class Interface
//{
//	public:
//		virtual void start(void) = 0;
//		virtual void stop(void) = 0;
//};

class Output// : public Interface
{
	public:
		Output(MAX14661* max, GPIO_Pin_t led, uint8_t a, uint8_t b)
		:	_router(max), _indication_LED(led), _maskA_pos(a), _maskB_pos(b)
		{	HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_RESET);}
		void start(void)// override
		{
			_router->setChannelA(_maskA_pos);
			_router->setChannelB(_maskB_pos);
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_SET);
		}
		void stop(void)// override
		{
			_router->clearChannelA(_maskA_pos);
			_router->clearChannelB(_maskB_pos);
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_RESET);
		}
	private:
		MAX14661* _router;
		GPIO_Pin_t _indication_LED;
		uint8_t _maskA_pos, _maskB_pos;			// connection mask on MAX14661 IC
};

class OutputControl
{
	public:
		OutputControl(Button* button, Output* output)
		:	_sw(button), _output(output)
		{	}
		void process(void);
		bool getState();
	private:
		Button* _sw;
		Output* _output;
		uint64_t _on_time;
		GPIO_PinState _state;
};
