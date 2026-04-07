#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>

#include "../board/board.h"
#include "../drivers/twi.h"
#include "../devices/lcd_hd44780_i2c.h"

bool ina_b1_st = false;
bool ina_b2_st = false;

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