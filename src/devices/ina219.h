#pragma once

#include <stdint.h>

/**
 * Initialize INA219 with the default configuration
 *
 * @param addr Device address
 */
void ina219_init(uint8_t addr);

/**
 * Read bus voltage in volts.
 *
 * @param addr Device address
 * @return
 */
float ina219_read_bus_voltage(uint8_t addr);

/**
 * Read current in amperes.
 *
 * @param addr Device address
 * @return
 */
float ina219_read_current(uint8_t addr);