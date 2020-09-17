#pragma once
#include <stdint.h>
class batt_gauge_ltc2943 
{

#define LTC2943_I2C_ADDR   0x64
public:
	// Enumeration of LTC2943 ADC modes
	typedef enum
	{
		AUTOMATIC = 3,
		SCAN = 2,
		MANUAL = 1,
		SLEEP = 0,
		NONE = 255			// Will indicate errors in adc mode getter
	} adc_mode_t;

	// Status for API calls
	typedef enum
	{
		STATUS_ERR,
		STATUS_OK
	} ltc_status_t;

	// Will set selected ADC mode for chip
	ltc_status_t set_adc_mode(adc_mode_t mode, i2c_driver& driver);
	// Will get current ADC mode on chip
	ltc_status_t get_adc_mode(i2c_driver& driver, adc_mode_t& output);
	// Checks if temperature alert is pending
	ltc_status_t is_temp_alert_pending(i2c_driver& driver, bool& output);
	// Checks if voltage alert is pending
	ltc_status_t is_volt_alert_pending(i2c_driver& driver, bool& output);
private:

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

	ltc_status_t	read_register(i2c_driver& driver, ltc2943_register_t reg, uint8_t* data_out, uint8_t size);
	ltc_status_t	write_register(i2c_driver& driver, ltc2943_register_t reg, uint8_t* data, uint8_t size);
	ltc_status_t	check_i2c_init(i2c_driver& driver);

};