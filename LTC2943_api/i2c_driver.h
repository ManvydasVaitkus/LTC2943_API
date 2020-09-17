#pragma once
#include <stdint.h>
class i2c_driver
{
public:
	// Constructor
	i2c_driver(void);
	// Destructor
	~i2c_driver(void);

	// Other API functions 
	bool i2cInit(void);

	bool i2cDeinit(void);

	bool i2cIsInitialized(void);

	bool i2cRead(uint8_t address, uint8_t *pDst, uint16_t dataSize); // Read data from I2C device to pDst buffer

	bool i2cWrite(uint8_t address, const uint8_t *pSrc, uint16_t dataSize); // Write data from pSrc buffer to I2C device

protected:

};