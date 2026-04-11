#pragma once

#include <stdint.h>

/**
 * RTC Read time
 *
 * @return
 */
void rtc_read_time(void);

/**
 * RTC Read date
 *
 * @return
 */
void rtc_read_date(void);

/**
 * RTC Init date time
 *
 * @return
 */
void rtc_init(void);

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

/**
 * Get day
 *
 * @return
 */
uint8_t get_day(void);

/**
 * Get month
 *
 * @return
 */
uint8_t get_month(void);

/**
 * Get year
 *
 * @return
 */
uint8_t get_year_from_2000(void);

/**
 * Get unixtime
 *
 * @return
 */
uint32_t get_unixtime(void);