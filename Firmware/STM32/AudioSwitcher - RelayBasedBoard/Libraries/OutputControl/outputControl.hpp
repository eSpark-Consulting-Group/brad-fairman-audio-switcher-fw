#pragma once

#include "main.h"
#include "button.hpp"


class Output : public Interface
{
	public:
		Output(GPIO_Pin_t relay, GPIO_Pin_t led)
		:	_relay(relay), _indication_LED(led)
		{	this->stop();	}

		void start(void) override
		{
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_relay.port, _relay.pin, GPIO_PIN_SET);
			_state = true;
		}
		void stop(void) override
		{
			HAL_GPIO_WritePin(_indication_LED.port, _indication_LED.pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_relay.port, _relay.pin, GPIO_PIN_RESET);
			_state = false;
		}
		bool isActive() override
		{
			return _state;
		}


	private:
		GPIO_Pin_t _relay;
		GPIO_Pin_t _indication_LED;

		bool _state;
};

typedef struct
{
	Interface* output[NUM_OF_OUTPUTS+1];
}OUTPUTS_s;

typedef enum
{
	FRONT_MODE,
	REAR1_MODE,
	CEN_REAR1_2_MODE,
	SUBWOOFER_MODE,
	FRONT_SUBWOOFER_MODE,
	REAR1_SUBWOOFER_MODE,
	CEN_REAR1_2_SUBWOOFER_MODE,
	ALL_ACTIVE_MODE,
	ALL_OFF_MODE,
	NO_CHANGE,
}OUTPUT_MODE_e;

//typedef enum
//{
//	ADD_OUTPUT,
//	REMOVE_OUTPUT,
//	MODE_CHANGE,
//}METHOD_e;

class OutputControl
{
	public:
		OutputControl(OUTPUT_MODE_e* mode)
		:_active_outputs(0), _on_time(0), _mode(mode),  _state(GPIO_PIN_RESET)
		{	*_mode = ALL_OFF_MODE;
			for(int i=0; i<(NUM_OF_OUTPUTS+1); i++)
			{
				_output.output[i] = NULL;
			}
		}
		void activate(void);
		void process(void);
		bool getState();
		uint8_t getMode(uint8_t buttonNumber);

		void addOutput(Interface* output);
		void removeAllOutputs();

	private:
		void turnOffAllOutputs();

		uint8_t	_active_outputs;
		uint64_t _on_time;
		OUTPUT_MODE_e* _mode;

		OUTPUTS_s _output;
		GPIO_PinState _state;
};
