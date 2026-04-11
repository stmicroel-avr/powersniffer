#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "timers.h"
#include "uart.h"
#include "../board/board.h"
#include "../devices/ina219.h"
#include "../devices/ds3231.h"
#include "../devices/lcd_hd44780_i2c.h"

static char uart_buffer[4][33];
static uint8_t uart_buffer_pos = 0;

/**
 * Show INA sensor values on display
 *
 * @param voltage Voltage value
 * @param current Current value
 * @param row1_pos Row1 position
 * @param row2_pos Row2 position
 */
static void display_sensor_value(float voltage, float current, uint8_t row1_pos, uint8_t row2_pos) {
    char row1[LCD_COLS + 1];
    char row2[LCD_COLS + 1];
    snprintf(row1, sizeof(row1), "B1 Voltage: %.2fV", voltage);
    snprintf(row2, sizeof(row2), "B1 Current: %.3fA", current);
    lcd_puts(row1_pos, row1);
    lcd_puts(row2_pos, row2);
}

/**
 * Read and display data for common mode
 *
 * @param ina_b1_st First INA state
 * @param ina_b2_st Second INA state
 * @param show_header Show the header row
 */
void display_common_mode(bool ina_b1_st, bool ina_b2_st, bool show_header) {
    if (show_header && (!ina_b1_st || !ina_b2_st)) {
        lcd_puts(1, "==== Common mode ====");
    }

    if (ina_b1_st) {
        display_sensor_value(
            ina219_read_bus_voltage(INA219_ADDR_B1),
            ina219_read_current(INA219_ADDR_B1),
            ina_b2_st ? 1 : 2,
            ina_b2_st ? 2 : 3
        );
    }

    if (ina_b2_st) {
        display_sensor_value(
           ina219_read_bus_voltage(INA219_ADDR_B2),
           ina219_read_current(INA219_ADDR_B2),
           ina_b1_st ? 3 : 1,
           ina_b1_st ? 4 : 2
       );
    }
}

/**
 * Read and display data for common mode
 *
 * @param ina_b1_st First INA state
 * @param ina_b2_st Second INA state
 * @param show_header Show the header row
 */
void display_ble_mode(bool ina_b1_st, bool ina_b2_st, bool show_header) {
    if (show_header && (!ina_b1_st || !ina_b2_st)) {
        lcd_puts(1, "=== BT send mode ===");
    }

    if (uart_buffer_pos == 3) {
        char buf[45];
        const uint32_t ut = get_unixtime();
        for (uint8_t i = 0; i <= 3; i++) {
            snprintf(buf, sizeof(buf), "%lu::%s", ut, uart_buffer[i]);
            uart_print(buf);
        }

        uart_buffer_pos = 0;
    }

    char buf[33] = "";

    if (ina_b1_st) {
        float v = ina219_read_bus_voltage(INA219_ADDR_B1);
        float c = ina219_read_current(INA219_ADDR_B1);
        display_sensor_value(v,c,ina_b2_st ? 1 : 2,ina_b2_st ? 2 : 3);
        uint8_t len = strlen(buf);
        snprintf(
            buf + len,
            sizeof(buf) - len,
            "b1:%.2fV,%.3fA;",
            v,
            c
        );
    }

    if (ina_b2_st) {
        float v = ina219_read_bus_voltage(INA219_ADDR_B2);
        float c = ina219_read_current(INA219_ADDR_B2);
        display_sensor_value(v,c,ina_b1_st ? 3 : 1,ina_b1_st ? 4 : 2);
        uint8_t len = strlen(buf);
        snprintf(
            buf + len,
            sizeof(buf) - len,
            "b2:%.2fV,%.3fA;",
            v,
            c
        );
    }

    snprintf(
        uart_buffer[uart_buffer_pos],
        sizeof(uart_buffer[uart_buffer_pos]),
        "%s",
        buf
    );
    uart_buffer_pos++;
}
