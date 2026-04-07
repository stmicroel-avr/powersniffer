#include <stdio.h>
#include <util/delay.h>

#include "board/board.h"
#include "drivers/twi.h"
#include "drivers/led.h"
#include "drivers/uart.h"
#include "devices/ina219.h"
#include "devices/lcd_hd44780_i2c.h"

#include "modules/app_state.h"
#include "modules/health_checks.h"
#include "modules/settings_mode.h"

/**
 * Main entry
 */
int main(void) {
    // Blink led on power on
    led_init();
    led_blink(1, LED_MODE_LONG);

    // I2C Init
    twi_init(I2C_BUS_FREQ);

    // UART Init
    uart_init(UART_BAUD);

    // LCD Init
    lcd_init();

    // Start screen
    lcd_puts(1, "PowerSniffer 1.0");
    lcd_puts(2, "By unstageable");
    lcd_puts(3, "Initialization...");

    // Check I2C dependencies
    if (!i2c_health_checks()) led_blink(UINT8_MAX, LED_MODE_INFINITE);

    // Settings mode
    SETTINGS_DDR &= ~(1 << SETTINGS_GPIO);
    if (SETTING_PIN & (1 << SETTINGS_GPIO)) settings_mode();

    // Initialize current/voltage meter
    ina219_init(INA219_ADDR_B1);
    ina219_init(INA219_ADDR_B2);

    // Blink
    led_blink(3, LED_DELAY_SHORT);

    // ISR
    btn_mode_init();

    // Startup delay
    _delay_ms(1000);

    // Clear display
    lcd_clear();

    // INA placeholders
    if (!ina_b1_st) {
        lcd_puts(1, "B1 Voltage: -");
        lcd_puts(1, "B1 Current: -");
    }
    // if (!ina_b2_st) {
    //     lcd_puts(3, "B2 Voltage: -");
    //     lcd_puts(4, "B2 Current: -");
    // }

    char buffer[LCD_ROWS][LCD_COLS + 1];
    while (1) {
        if (ina_b1_st) {
            float current = ina219_read_current(INA219_ADDR_B1);
            float voltage = ina219_read_bus_voltage(INA219_ADDR_B1);
            snprintf(buffer[0], sizeof(buffer[0]), "B1 Voltage: %.2fV", voltage);
            snprintf(buffer[1], sizeof(buffer[1]), "B1 Current: %.3fA", current);
            lcd_puts(1, buffer[0]);
            lcd_puts(2, buffer[1]);
        }

        if (ina_b2_st) {
            float current = ina219_read_current(INA219_ADDR_B2);
            float voltage = ina219_read_bus_voltage(INA219_ADDR_B2);
            snprintf(buffer[2], sizeof(buffer[0]), "B2 Voltage: %.2fV", voltage);
            snprintf(buffer[3], sizeof(buffer[1]), "B2 Current: %.3fA", current);
            lcd_puts(3, buffer[2]);
            lcd_puts(4, buffer[3]);
        }

        snprintf(buffer[3], sizeof(buffer[3]), "App mode: %d", app_mode);
        lcd_puts(4, buffer[3]);

        _delay_ms(200);
    }
}