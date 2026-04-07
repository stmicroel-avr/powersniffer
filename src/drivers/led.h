#pragma once

#include <stdint.h>

/**
 * Initialize led port
 */
void led_init(void);

/**
 * Blink led
 *
 * @param num Number of blinks
 * @param mode Blink mode
 */
void led_blink(uint8_t num, uint8_t mode);