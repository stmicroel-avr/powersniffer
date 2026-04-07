#pragma once

#include <stdint.h>

extern volatile uint8_t app_mode;

/**
 * Allow button ISR
 */
void btn_mode_init(void);