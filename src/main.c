#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>

#include "board/board.h"
#include "drivers/twi.h"
#include "drivers/led.h"
#include "drivers/uart.h"
#include "devices/ina219.h"
#include "drivers/timers.h"
#include "devices/lcd_hd44780_i2c.h"

static bool ina_b1_st = false;
static bool ina_b2_st = false;

/**
 * I2C Checks
 */
bool health_checks(void) {
    uint8_t devices[5] = {
        LCD_I2C_ADDR,
        DS3231_ADDR,
        EXT_EEPROM_ADDR,
        INA219_ADDR_B1,
        INA219_ADDR_B2,
    };

    for (uint8_t i = 0; i < 5; i++) {
        bool stop = false;
        char line[LCD_COLS + 1];
        const char *name = "Unknown";

        bool dev_st = twi_check_addr(devices[i]);

        if (devices[i] == LCD_I2C_ADDR) {
            if (!dev_st) return false;
            name = "PCF8574 LCD";
        } else if (devices[i] == DS3231_ADDR) {
            name = "DS3231 RTC";
            if (!dev_st) stop = true;
        } else if (devices[i] == EXT_EEPROM_ADDR) {
            name = "AT24XX EEPROM";
            if (!dev_st) stop = true;
        } else if (devices[i] == INA219_ADDR_B1) {
            name = "INA219(BUS1)";
            ina_b1_st = dev_st;
        } else if (devices[i] == INA219_ADDR_B2) {
            name = "INA219(BUS2)";
            ina_b2_st = dev_st;
        }

        snprintf(line, sizeof(line), "%s %s", name, dev_st ? "OK" : "FAIL");
        lcd_puts(4, line);

        if (stop) return false;

        _delay_ms(750);
    }

    return ina_b1_st || ina_b2_st;
}

/**
 * Run settings (HC-05 ble set name/pass)
 */
void run_settings(void) {
    char response[21];

    lcd_clear();
    _delay_ms(100);
    lcd_puts(1, "Settings mode");

    // --- AT ---
    lcd_puts(2, "Cmd: AT");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, " ");

    uart_print("AT\r\n");
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK");
    } else {
        lcd_puts(3, "Status: Failed(stop)");
        while (1);
    }
    _delay_ms(1500);

    // --- VERSION ---
    lcd_puts(2, "Cmd: AT+VERSION?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, " ");

    uart_print("AT+VERSION?\r\n");
    if (uart_read_line(response, sizeof(response), 2000)) {
        lcd_puts(3, "Status: OK");
        lcd_puts(4, response);
    } else {
        lcd_puts(3, "Status: Failed");
    }
    uart_expect("OK", 1000);
    _delay_ms(1500);

    // --- ADDR ---
    lcd_puts(2, "Cmd: AT+ADDR?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, "  ");

    uart_print("AT+ADDR?\r\n");
    if (uart_read_line(response, sizeof(response), 2000)) {
        lcd_puts(3, "Status: OK");
        lcd_puts(4, response);
    } else {
        lcd_puts(3, "Status: Failed");
    }
    uart_expect("OK", 1000);
    _delay_ms(1500);

    // --- NAME ---
    lcd_puts(2, "Cmd: AT+NAME?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, "  ");

    uart_print("AT+NAME?\r\n");
    if (uart_read_line(response, sizeof(response), 2000)) {
        lcd_puts(3, "Status: OK");
        lcd_puts(4, response);
    } else {
        lcd_puts(3, "Status: Failed");
    }
    uart_expect("OK", 1000);
    _delay_ms(1500);

    lcd_puts(2, "Settings done");
    lcd_puts(3, "All data read");
    lcd_puts(4, " ");

    _delay_ms(2000);

    char cmd[30] = "";
    strcat(cmd, "AT+NAME=");
    strcat(cmd, HC05_NETWORK_NAME);
    strcat(cmd, "\r\n");


    // --- SET NAME ---
    lcd_puts(2, "Cmd: SET NAME");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, " ");

    uart_print(cmd);
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK");
    } else {
        lcd_puts(3, "Status: Failed");
    }
    _delay_ms(1500);

    memset(cmd, 0, sizeof(cmd));
    strcat(cmd, "AT+PSWD=");
    strcat(cmd, HC05_NETWORK_PASS);
    strcat(cmd, "\r\n");

    // --- SET PASSWORD ---
    lcd_puts(2, "Cmd: SET PASSWORD  ");
    lcd_puts(3, "Wait response...   ");
    lcd_puts(4, " ");

    uart_print(cmd);
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK         ");
    } else {
        lcd_puts(3, "Status: Failed     ");
    }
    _delay_ms(1500);

    // --- DONE ---
    lcd_puts(2, "Settings done");
    lcd_puts(3, "Saved");
    lcd_puts(4, "Please reboot");

    while (1);
}

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
    if (!health_checks()) led_blink(UINT8_MAX, LED_MODE_INFINITE);

    // Settings mode
    SETTINGS_DDR &= ~(1 << SETTINGS_GPIO);
    if (SETTING_PIN & (1 << SETTINGS_GPIO)) run_settings();

    // Initialize current/voltage meter
    ina219_init(INA219_ADDR_B1);
    ina219_init(INA219_ADDR_B2);

    // Blink
    led_blink(3, LED_DELAY_SHORT);

    // Timer init
    timer_init();

    // Startup delay
    _delay_ms(1000);

    // Clear display
    lcd_clear();

    // INA placeholders
    if (!ina_b1_st) {
        lcd_puts(1, "B1 Voltage: -");
        lcd_puts(1, "B1 Current: -");
    }
    if (!ina_b2_st) {
        lcd_puts(3, "B2 Voltage: -");
        lcd_puts(4, "B2 Current: -");
    }

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

        _delay_ms(250);
    }
}