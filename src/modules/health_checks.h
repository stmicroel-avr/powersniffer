#pragma once

#include <stdbool.h>

// Glob states
extern bool ina_b1_st;
extern bool ina_b2_st;

/**
 * I2C devices checks
 *
 * @return bool
 */
bool i2c_health_checks(void);