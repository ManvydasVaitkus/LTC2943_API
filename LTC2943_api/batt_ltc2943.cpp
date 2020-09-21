/*
LTC2943 chip is connected on I2C peripheral and you have peripheral driver written.

Your task is to create and implement very simple chip control module API.Using your API other programmers should be able to :

1.       Change chip's ADC mode into one of these modes: automatic mode, scan mode, manual mode, sleep.

2.       Get chip's ADC mode.

3.       Check if temperature alert is pending.

4.       Check if voltage alert is pending.

Datasheet: http://cds.linear.com/docs/en/datasheet/2943fa.pdf



I2C driver interface looks like this :
bool i2cInit(void);
bool i2cDeinit(void);
bool i2cIsInitialized(void);
bool i2cRead(uint8_t address, uint8_t *pDst, uint16_t dataSize); // Read data from I2C device to pDst buffer
bool i2cWrite(uint8_t address, const uint8_t *pSrc, uint16_t dataSize); // Write data from pSrc buffer to I2C device

*/
#include "i2c_driver.h"
#include "batt_ltc2943.h"
#include <vector>
using namespace std;


// Will set selected ADC mode for chip
batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::set_adc_mode(ADCMode mode)
{
	LTCStatus status = LTCStatus::STATUS_ERR;
	control_reg data;
	uint8_t mode_bits = 0x00;
	uint8_t out = 0x00;

	// Only execute if i2c is initialized 
	if ((check_i2c_init() != LTCStatus::STATUS_OK) || (mode == ADCMode::NONE))
	{
		return LTCStatus::STATUS_ERR;
	}

	// ADC mode is set by programing the CONTROL register bits B[7:6]
	// [11] Automatic; [10] Scan; [01] Manual; [00] Sleep

	// Read, to not override other parameters 
	if (read_register(CONTROL, &out, 1) != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}
	data.all = out;

	//Edit 
	data.reg_val.adc_mode = (uint8_t) mode;

	// Write
	return write_register(CONTROL, &data.all, 1);

}


// Will get current ADC mode on chip
batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::get_adc_mode(ADCMode& output)
{
	ADCMode adc_mode = ADCMode::NONE;
	control_reg data;
	uint8_t out = 0x00;
	
	// Only execute if i2c is initialized 
	if (check_i2c_init() != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}
	
	// ADC mode is get by reading the CONTROL register bits B[7:6]
	if (read_register(CONTROL, &out, 1) != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}

	data.all = out;
	output = (ADCMode) data.reg_val.adc_mode;
	return LTCStatus::STATUS_OK;
}


// Checks if temperature alert is pending
batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::is_temp_alert_pending(bool& output)
{
	uint8_t data = 0x00;
	// Only execute if i2c is initialized 
	if (check_i2c_init() != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}

	// Temperature alert is in STATUS Register bit A[4]
	// Get status register
	if (read_register(STATUS, &data, 1) != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}
	// Check coresponding bit
	output = (bool)((data >> 4) & 0x01);

	return LTCStatus::STATUS_OK;
}


// Checks if voltage alert is pending
batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::is_volt_alert_pending(bool& output)
{
	uint8_t data = 0x00;
	// Only execute if i2c is initialized 
	if (check_i2c_init() != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}
	
	// Voltage alert is in STATUS Register bit A[1]
	// Get status register
	if (read_register(STATUS, &data, 1) != LTCStatus::STATUS_OK)
	{
		return LTCStatus::STATUS_ERR;
	}
	// Check coresponding bit
	output = (bool)((data >> 1) & 0x01);

	return LTCStatus::STATUS_OK;
}

batt_gauge_ltc2943::batt_gauge_ltc2943(i2c_driver* dr)
{
	driver = dr;
}


batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::read_register(ltc2943_register_t reg, uint8_t* data_out, uint8_t size)
{
	const uint8_t write_data = reg;
	// For reading we first write
	if (!driver->i2cWrite(LTC2943_I2C_ADDR, &write_data, 1))
	{
		return LTCStatus::STATUS_ERR;
	}
	// Read values
	if (!driver->i2cRead(LTC2943_I2C_ADDR, data_out, size))
	{
		return LTCStatus::STATUS_ERR;
	}


	return LTCStatus::STATUS_ERR;
}


batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::write_register(ltc2943_register_t reg, uint8_t* data, uint8_t size)
{
	// Form data
	vector<uint8_t> buff; 
	buff[0] = (uint8_t) reg;
	
	// Copy data to buff until size or buff maximum size 
	for (uint8_t i = 0; i < size; i++)
	{
		buff.push_back(*(data + i));
	}

	// Write data through I2C
	if (!driver->i2cWrite(LTC2943_I2C_ADDR, &buff[0], size + 1))
	{
		return LTCStatus::STATUS_ERR;
	}

	return LTCStatus::STATUS_OK;
}


batt_gauge_ltc2943::LTCStatus batt_gauge_ltc2943::check_i2c_init()
{
	if (!driver->i2cIsInitialized())
	{
		// If not try to init
		if (!driver->i2cInit())
		{
			// Can't init - return error
			return LTCStatus::STATUS_ERR;
		}
	}
	return LTCStatus::STATUS_OK;
}