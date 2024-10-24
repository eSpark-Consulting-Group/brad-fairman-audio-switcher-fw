#include "MAX14661.h"


//typedef enum
//{
//  HAL_OK       = 0x00U,
//  HAL_ERROR    = 0x01U,
//  HAL_BUSY     = 0x02U,
//  HAL_TIMEOUT  = 0x03U
//} HAL_StatusTypeDef;


//
//  REGISTERS
//
#define MAX14661_DIR0       0x00
#define MAX14661_DIR1       0x01
#define MAX14661_DIR2       0x02
#define MAX14661_DIR3       0x03
#define MAX14661_SHDW0      0x10
#define MAX14661_SHDW1      0x11
#define MAX14661_SHDW2      0x12
#define MAX14661_SHDW3      0x13
#define MAX14661_CMD_A      0x14
#define MAX14661_CMD_B      0x15


MAX14661::MAX14661(I2C_HandleTypeDef *wire, const uint8_t deviceAddress)
{
	_address = deviceAddress;
	_i2c    = wire;
	_error   = MAX14661_OK;
}


bool MAX14661::begin()
{
	_error = HAL_I2C_Init(_i2c);
	return _error;
}


bool MAX14661::isConnected()
{
	_error = HAL_I2C_IsDeviceReady(_i2c, (_address<<1), 3, 5);
//	_wire->beginTransmission(_address);
//	_error = _wire->endTransmission();
	//  set MAX14661_ERR_I2C
	return ( _error == MAX14661_OK);
}


uint8_t MAX14661::getAddress()
{
  return _address;
}


/////////////////////////////////////////////////////////
//
//  PAIR INTERFACE
//
bool MAX14661::openChannel(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR0;
  if (ch > 7)
  {
    reg = MAX14661_DIR1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  reg += 2;
  mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::closeChannel(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR0;
  if (ch > 7)
  {
    reg = MAX14661_DIR1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  reg += 2;
  mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  return true;
}


//  assumption both A and B are in same state
bool MAX14661::isOpenChannel(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR0;
  if (ch > 7)
  {
    reg = MAX14661_DIR1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  return (mask & (1 << ch)) > 0;
}


void MAX14661::openAllChannels()
{
  setChannels(0xFFFF);
}


void MAX14661::closeAllChannels()
{
  setChannels(0);
}


void MAX14661::setChannels(uint16_t mask)
{
  writeRegister(MAX14661_DIR0, mask & 0x00FF);
  writeRegister(MAX14661_DIR1, (mask & 0xFF00) >> 8);
  writeRegister(MAX14661_DIR2, mask & 0x00FF);
  writeRegister(MAX14661_DIR3, (mask & 0xFF00) >> 8);
}

void MAX14661::setChannelA(uint8_t pos)
{
  uint16_t channelState = getChannelsA();
  channelState |= (1<<pos);
  writeRegister(MAX14661_DIR0, (channelState & 0x00FF));
  writeRegister(MAX14661_DIR1, (channelState & 0xFF00) >> 8);
}
void MAX14661::setChannelB(uint8_t pos)
{
  uint16_t channelState = getChannelsB();
  channelState |= (1<<pos);
  writeRegister(MAX14661_DIR2, (channelState & 0x00FF));
  writeRegister(MAX14661_DIR3, (channelState & 0xFF00) >> 8);
}

void MAX14661::clearChannelA(uint8_t pos)
{
  uint16_t channelState = getChannelsA();
  channelState &= ~(1 << pos);
  writeRegister(MAX14661_DIR0, (channelState & 0x00FF));
  writeRegister(MAX14661_DIR1, (channelState & 0xFF00) >> 8);
}
void MAX14661::clearChannelB(uint8_t pos)
{
  uint16_t channelState = getChannelsB();
  channelState &= ~(1 << pos);
  writeRegister(MAX14661_DIR2, (channelState & 0x00FF));
  writeRegister(MAX14661_DIR3, (channelState & 0xFF00) >> 8);
}



uint16_t MAX14661::getChannelsA()
{
  uint16_t channels = readRegister(MAX14661_DIR1) << 8;
  channels |= readRegister(MAX14661_DIR0);
  return channels;
}

uint16_t MAX14661::getChannelsB()
{
  uint16_t channels = readRegister(MAX14661_DIR3) << 8;
  channels |= readRegister(MAX14661_DIR2);
  return channels;
}


/////////////////////////////////////////////////////////
//
//  SHADOW INTERFACE
//
void MAX14661::shadowClear()
{
  setShadowChannelMaskA(0x0000);
  setShadowChannelMaskB(0x0000);
}


void MAX14661::activateShadow()
{
  writeRegister(MAX14661_CMD_A, 0x11);
  writeRegister(MAX14661_CMD_B, 0x11);
}


////////////////////////////////////////////////////////////


void MAX14661::setShadowChannelMaskA(uint16_t mask)
{
  writeRegister(MAX14661_SHDW0, mask & 0xFF);
  writeRegister(MAX14661_SHDW1, mask >> 8);
}


uint16_t MAX14661::getShadowChannelMaskA()
{
  uint16_t mask = readRegister(MAX14661_SHDW1) << 8;
  mask |= readRegister(MAX14661_SHDW0);
  return mask;
}


void MAX14661::setShadowChannelMaskB(uint16_t mask)
{
  writeRegister(MAX14661_SHDW2, mask & 0xFF);
  writeRegister(MAX14661_SHDW3, mask >> 8);
}


uint16_t MAX14661::getShadowChannelMaskB()
{
  uint16_t mask = readRegister(MAX14661_SHDW3) << 8;
  mask |= readRegister(MAX14661_SHDW2);
  return mask;
}


////////////////////////////////////////////////////////////


bool MAX14661::openShadowChannelA(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW0;
  if (ch > 7)
  {
    reg = MAX14661_SHDW1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::closeShadowChannelA(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW0;
  if (ch > 7)
  {
    reg = MAX14661_SHDW1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::isOpenShadowChannelA(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW0;
  if (ch > 7)
  {
    reg = MAX14661_SHDW1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  return (mask & (1 << ch)) > 0;
}


bool MAX14661::openShadowChannelB(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW0;
  if (ch > 7)
  {
    reg = MAX14661_SHDW1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::closeShadowChannelB(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW2;
  if (ch > 7)
  {
    reg = MAX14661_SHDW3;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::isOpenShadowChannelB(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_SHDW2;
  if (ch > 7)
  {
    reg = MAX14661_SHDW3;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  return (mask & (1 << ch)) > 0;
}



/////////////////////////////////////////////////////////
//
//  MUX INTERFACE
//
void MAX14661::MUXA(uint8_t channel)
{
  uint8_t ch = channel;
  if (channel > 15) ch = 0x10;
  writeRegister(MAX14661_CMD_A, ch);
}


uint8_t MAX14661::getMUXA()
{
  uint8_t ch = readRegister(MAX14661_CMD_A);
  if ((ch & 0x10) == 0) return (ch & 0x0F);
  return 255;
}


void MAX14661::MUXB(uint8_t channel)
{
  uint8_t ch = channel;
  if (channel > 15) ch = 0x10;
  writeRegister(MAX14661_CMD_B, ch);
}


uint8_t MAX14661::getMUXB()
{
  uint8_t ch = readRegister(MAX14661_CMD_B);
  if ((ch & 0x10) == 0) return (ch & 0x0F);
  return 255;
}


/////////////////////////////////////////////////////////
//
//  FULL CONTROL
//
bool MAX14661::openA(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR0;
  if (ch > 7)
  {
    reg = MAX14661_DIR1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::openB(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR2;
  if (ch > 7)
  {
    reg = MAX14661_DIR3;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask |= (1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::closeA(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR0;
  if (ch > 7)
  {
    reg = MAX14661_DIR1;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  return true;
}


bool MAX14661::closeB(uint8_t channel)
{
  if (channel > 15) return false;
  uint8_t ch = channel;
  uint8_t reg = MAX14661_DIR2;
  if (ch > 7)
  {
    reg = MAX14661_DIR3;
    ch -= 8;
  }
  uint8_t mask = readRegister(reg);
  mask &= ~(1 << ch);
  writeRegister(reg, mask);
  return true;
}


/////////////////////////////////////////////////////////
//
//  LOW LEVEL CONTROL
//
uint8_t MAX14661::readRegister(uint8_t reg)
{
//	_wire->beginTransmission(_address);
//	_wire->write(reg);
//	_error = _wire->endTransmission();
	_error = HAL_I2C_Master_Transmit(_i2c, (_address<<1), &reg, sizeof(reg), 5);
	if ( MAX14661_OK != _error )
	{
		_error = MAX14661_ERR_I2C;
		return 0;
	}
	uint8_t data = 0;
	_error = HAL_I2C_Master_Receive(_i2c, (_address<<1), &data, sizeof(data), 5);
	if ( MAX14661_OK != _error )
	{
		_error = MAX14661_ERR_I2C;
		return 0;
	}
	return data;
}


int MAX14661::writeRegister(uint8_t reg, uint8_t value)
{
	_error = HAL_I2C_Master_Transmit(_i2c, (_address<<1), &reg, sizeof(reg), 5);
	if( MAX14661_OK == _error )
		_error = HAL_I2C_Master_Transmit(_i2c, (_address<<1), &value, sizeof(value), 5);
	return _error;
}


int MAX14661::lastError()
{
  int e = _error;
  _error = MAX14661_OK;
  return e;
}


//  -- END OF FILE --

