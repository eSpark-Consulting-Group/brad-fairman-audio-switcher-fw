
#include "LM1972.h"

#define MAX_ATTENUATION		78

LM1972::LM1972(GPIO_Pin_t din, GPIO_Pin_t sck, GPIO_Pin_t cs, Button* up, Button* dwn)
//: _spi(spi), _csPin(cs), _up_SW(up), _dwn_SW(dwn), level(0)
: _din(din), _sck(sck), _csPin(cs), _up_SW(up), _dwn_SW(dwn), level(0)
{
	HAL_GPIO_WritePin(_csPin.port, _csPin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_din.port, _din.pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(_sck.port, _sck.pin, GPIO_PIN_RESET);
	this->setAttenuationDB(level);
}

void LM1972::processButtons()
{
	_up_SW->update();
	_dwn_SW->update();

	if((GPIO_PIN_RESET != _up_SW->getButtonState()) && (_upSwState == GPIO_PIN_RESET))
	{
		if(level > 0 )
		{	this->setAttenuationDB((MAX_ATTENUATION/10)*(--level));	}
		_upSwState = GPIO_PIN_SET;
	}
    else
    {
		_upSwState = GPIO_PIN_RESET;
    }

	if((GPIO_PIN_RESET != _dwn_SW->getButtonState()) && (_dwnSwState == GPIO_PIN_RESET))
	{
		if(level < 10 )
		{	this->setAttenuationDB((MAX_ATTENUATION/10)*(++level));	}
		_dwnSwState = GPIO_PIN_SET;
	}
    else
    {
		_dwnSwState = GPIO_PIN_RESET;
    }
}

void LM1972::setAttenuationDB(uint8_t decibel)
{
    // Default: Mute, actually -104dB, displayed as -79dB.
    uint8_t attenuation = 128;

    // -48dB to -78dB: 1dB per step.
    if (decibel >= 48 && decibel <= 78)
    {
        // Attenuation levels 96 to 126
        attenuation = (decibel - 48) + 96;
    }

    // -0dB to -48dB: 0.5dB per step.
    if (decibel < 48)
    {
        // Attenuation levels 0 to 96
        attenuation = (decibel * 2);
    }

    this->setAttenuation(attenuation);
}

void LM1972::setAttenuation(uint8_t attenuation)
{
    this->set(channel1, attenuation);
    this->set(channel2, attenuation);
    this->set(channel3, attenuation);
    this->m_attenuation = attenuation;
}

uint8_t LM1972::getAttenuation()
{
    return this->m_attenuation;
}

uint8_t LM1972::getAttenuationDB()
{

    // Get shadowed attenuation.
    uint8_t attenuation = this->getAttenuation();

    // Default: Mute, -79dB (actually even -104dB).
    uint8_t decibel = 79;

    // Attenuation levels 96 to 126: 1dB per step.
    if (attenuation >= 96 && attenuation <= 126)
    {
        // -48dB to -78dB
        decibel = (attenuation - 96) + 48;
    }

    // Attenuation levels 0 to 96: 0.5dB per step.
    if (attenuation < 96)
    {
        // -0dB to -48dB
        decibel = (attenuation / 2);
    }

    return decibel;
}

void LM1972::set(LM1972::ADDRESS addr, uint8_t data)
{
	HAL_GPIO_WritePin(_csPin.port, _csPin.pin, GPIO_PIN_RESET);
//    m_clk.off();
    HAL_Delay(1);

    auto sendBits = [&](uint8_t value) {
        for (uint8_t i = 0; i < 8; i++) {
            GPIO_PinState bitValue = (value & (1 << (7 - i))) ? GPIO_PIN_SET : GPIO_PIN_RESET;

            HAL_GPIO_WritePin(_din.port, _din.pin, bitValue);
            HAL_Delay(1);
            HAL_GPIO_WritePin(_sck.port, _sck.pin, GPIO_PIN_SET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(_sck.port, _sck.pin, GPIO_PIN_RESET);
        }
    };

    sendBits(addr);
    sendBits(data);

//    uint8_t packet = addr;
//    while(HAL_SPI_Transmit(_spi, &packet, sizeof(packet), 5) != HAL_OK)
//
//    while(HAL_SPI_Transmit(_spi, &data, sizeof(data), 5) != HAL_OK);

    HAL_Delay(1);

	HAL_GPIO_WritePin(_csPin.port, _csPin.pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(_din.port, _din.pin, GPIO_PIN_RESET);
}
