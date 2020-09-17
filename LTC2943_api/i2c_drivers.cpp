#include "i2c_driver.h"

// Constructor
i2c_driver::i2c_driver(void)
{

}

// Destructor
i2c_driver::~i2c_driver(void)
{

}
// Initialization of I2C driver
bool i2c_driver::i2cInit(void)
{
	return false;
}

// Deinitialization of I2C driver
bool i2c_driver::i2cDeinit(void)
{
	return false;
}

// Check if i2c is initialized
bool i2c_driver::i2cIsInitialized(void)
{
	return false;
}

// Read data from I2C device to pDst buffer
bool i2c_driver::i2cRead(uint8_t address, uint8_t *pDst, uint16_t dataSize)
{
	return false;
}

// Write data from pSrc buffer to I2C device
bool i2c_driver::i2cWrite(uint8_t address, const uint8_t *pSrc, uint16_t dataSize)
{
	return false;
}