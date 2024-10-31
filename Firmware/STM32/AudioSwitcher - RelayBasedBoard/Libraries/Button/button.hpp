#ifndef BUTTONS
#define BUTTONS

#include "main.h"

#define SHORT_PRESS_TIME        250

typedef enum
{
	OUT1_SW,
	OUT2_SW,
	OUT3_SW,
	OUT4_SW,
	OUT5_SW,
	OUT6_SW,
}SW_e;

class Interface
{
	public:
		virtual void start(void) = 0;
		virtual void stop(void) = 0;
		virtual bool isActive(void) { return 0;	}
};

class Button : public Interface
{
    public:
        Button(GPIO_Pin_t pin, GPIO_Pin_t led, bool buttonType = true)
        : _pin(pin), _led_pin(led), _debounce_time(DEBOUNCE_TIME_MS), _last_debounce_time(0), _state(GPIO_PIN_SET),
		  _last_button_state(GPIO_PIN_SET), _button_press_time(0), _is_active(false),
		  _is_output_control_button(buttonType), _triggerred(false)
        {   HAL_GPIO_WritePin(_led_pin.port, _led_pin.pin, GPIO_PIN_SET);     }

        void update(void);
        void start(void) override {
        	setState(true);
        }
        void stop(void) override {
        	setState(false);
        }
        GPIO_PinState getButtonState();
        bool isActive() override;
        bool getType();
    private:
        void setState(bool state);

        GPIO_Pin_t     _pin, _led_pin;
        unsigned long  _debounce_time, _last_debounce_time;
        GPIO_PinState  _state, _last_button_state, _trigger;
        uint64_t       _button_press_time, _active_Toggle_Time;
        bool 		   _is_active, _is_output_control_button, _triggerred;
};


#endif
