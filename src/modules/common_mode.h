#pragma once

#include <stdbool.h>

/**
 * Read and display data for common mode
 *
 * @param ina_b1_st First INA state
 * @param ina_b2_st Second INA state
 * @param show_header Show the header row
 */
void display_common_mode(bool ina_b1_st, bool ina_b2_st, bool show_header);