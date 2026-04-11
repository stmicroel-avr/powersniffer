#include <stdio.h>
#include <util/delay.h>

#include "ds3231.h"
#include "board/board.h"
#include "drivers/twi.h"
#include "drivers/led.h"
#include "drivers/uart.h"
#include "drivers/timers.h"
#include "devices/ina219.h"
#include "devices/lcd_hd44780_i2c.h"

#include "modules/app_state.h"
#include "modules/health_checks.h"
#include "modules/settings_mode.h"
#include "modules/ina_modes.h"
#include "modules/clock_mode.h"

/**
 * Main entry
 */
int main(void) {
    // Blink led on power on
    led_init();
    led_blink(1, LED_MODE_LONG);

    // I2C Init
    twi_init(I2C_BUS_FREQ);

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

    // UART Init
    uart_init(UART_BAUD);

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

    // Init ms timers
    init_ms_timer();

    // Init RTC
    rtc_init();

    bool show_header = true;
    bool force_refresh = true;
    uint32_t last_touch_ms = 0;
    uint8_t last_mode = app_mode;
    uint32_t last_rtc_update_touch_ms = get_ms();

    while (1) {
        if (force_refresh || (get_ms() - last_touch_ms) >= REFRESH_RATE_MS) {
            if (app_mode == COMMON_MODE) {
                display_common_mode(ina_b1_st, ina_b2_st, show_header);
            } else if (app_mode == CLOCK_MODE) {
                display_clock(show_header);
            } else if (app_mode == METRICS_SEND_MODE) {
                display_ble_mode(ina_b1_st, ina_b2_st, show_header);
            }

            show_header = false;
            force_refresh = false;
            last_touch_ms = get_ms();
        }

        if (last_mode != app_mode) {
            last_mode = app_mode;
            force_refresh = true;
            show_header = true;
            lcd_clear();
        }

        if ((get_ms() - last_rtc_update_touch_ms) >= REFRESH_RTC_MS) {
            rtc_read_time();
            last_rtc_update_touch_ms = get_ms();
        }

        _delay_ms(1);
    }
}