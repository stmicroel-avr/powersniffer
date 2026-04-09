#pragma once

#include <stdint.h>

/**
 * Init 1ms timer
 */
void init_ms_timer(void);

/**
 * Get ms value
 *
 * @return
 */
uint32_t get_ms(void);