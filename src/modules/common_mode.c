#include <stdio.h>
#include <stdbool.h>

#include "../board/board.h"
#include "../devices/ina219.h"
#include "../devices/lcd_hd44780_i2c.h"

/**
 * Read and display data for common mode
 *
 * @param ina_b1_st First INA state
 * @param ina_b2_st Second INA state
 * @param show_header Show the header row
 */
void display_common_mode(bool ina_b1_st, bool ina_b2_st, bool show_header) {
    static uint8_t rows = 4;
    if (show_header) {
        if (!ina_b1_st || !ina_b2_st) {
            lcd_puts(1, "==== Common mode ====");
            rows = 2;
        }
    }

    char buffer[rows][LCD_COLS + 1];

    if (ina_b1_st) {
        float current = ina219_read_current(INA219_ADDR_B1);
        float voltage = ina219_read_bus_voltage(INA219_ADDR_B1);
        snprintf(buffer[0], sizeof(buffer[0]), "B1 Voltage: %.2fV", voltage);
        snprintf(buffer[1], sizeof(buffer[1]), "B1 Current: %.3fA", current);
        lcd_puts(ina_b2_st ? 1 : 2, buffer[0]);
        lcd_puts(ina_b2_st ? 2 : 3, buffer[1]);
    }

    if (ina_b2_st) {
        float current = ina219_read_current(INA219_ADDR_B2);
        float voltage = ina219_read_bus_voltage(INA219_ADDR_B2);
        uint8_t f_buffer_pos = ina_b1_st ? 2 : 0;
        uint8_t f_row_pos = ina_b1_st ? 3 : 1;
        snprintf(buffer[f_buffer_pos], sizeof(buffer[f_buffer_pos]), "B2 Voltage: %.2fV", voltage);
        snprintf(buffer[f_buffer_pos+1], sizeof(buffer[f_buffer_pos+1]), "B2 Current: %.3fA", current);
        lcd_puts(f_row_pos, buffer[f_buffer_pos]);
        lcd_puts(f_row_pos+1, buffer[f_buffer_pos+1]);
    }
}