#pragma once
#include <stdint.h>
class batt_gauge_ltc2943 
{
public:
	// Enumeration of LTC2943 ADC modes
	enum class ADCMode
	{
		AUTOMATIC = 3,
		SCAN = 2,
		MANUAL = 1,
		SLEEP = 0,
		NONE = 255			// Will indicate errors in adc mode getter
	};

	// Status for API calls
	enum class LTCStatus
	{
		STATUS_ERR,
		STATUS_OK
	};

	// Pointer to driver
	i2c_driver* driver;
	// Constructor
	batt_gauge_ltc2943(i2c_driver* dr);
	// Will set selected ADC mode for chip
	LTCStatus set_adc_mode(ADCMode mode);
	// Will get current ADC mode on chip
	LTCStatus get_adc_mode(ADCMode& output);
	// Checks if temperature alert is pending
	LTCStatus is_temp_alert_pending(bool& output);
	// Checks if voltage alert is pending
	LTCStatus is_volt_alert_pending(bool& output);
private:
	const uint8_t LTC2943_I2C_ADDR = 0x64;
	// Enumeration of LTC2943 Registers
	typedef enum
	{
		STATUS,
		CONTROL,
		ACCUM_CHARGE_MSB,
		ACCUM_CHARGE_LSB,
		CHRG_TRSH_HIGH_MSB,
		CHRG_TRSH_HIGH_LSB,
		CHRG_TRSH_LOW_MSB,
		CHRG_TRSH_LOW_LSB,
		VOLTAGE_MSB,
		VOLTAGE_LSB,
		VOLT_TRSH_HIGH_MSB,
		VOLT_TRSH_HIGH_LSB,
		VOLT_TRSH_LOW_MSB,
		VOLT_TRSH_LOW_LSB,
		CURRENT_MSB,
		CURRENT_LSB,
		CURRENT_TRSH_HIGH_MSB,
		CURRENT_TRSH_HIGH_LSB,
		CURRENT_TRSH_LOW_MSB,
		CURRENT_TRSH_LOW_LSB,
		TEMPERATURE_MSB,
		TEMPERATURE_LSB,
		TEMPERATURE_TRSH_HIGH,
		TEMPERATURE_TRSH_LOW
	} ltc2943_register_t;

	typedef struct
	{
		uint8_t shutdown : 1;
		uint8_t alcc_cfg : 2;
		uint8_t prescale_m : 3;
		uint8_t adc_mode : 2;
	} control_register_values_t;

	typedef union
	{
		control_register_values_t reg_val;
		uint8_t					  all;
	} control_reg;

	LTCStatus	read_register(ltc2943_register_t reg, uint8_t* data_out, uint8_t size);
	LTCStatus	write_register(ltc2943_register_t reg, uint8_t* data, uint8_t size);
	LTCStatus	check_i2c_init();

};