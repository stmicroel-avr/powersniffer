#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * RTC Read time
 *
 * @return
 */
bool rtc_read_time(void);

/**
 * Get hours
 *
 * @return
 */
uint8_t get_hours(void);

/**
 * Get minutes
 *
 * @return
 */
uint8_t get_minutes(void);

/**
 * Get seconds
 *
 * @return
 */
uint8_t get_seconds(void);