#include <stdio.h>
#include <stdbool.h>

#include "../board/board.h"
#include "../devices/ds3231.h"
#include "../devices/lcd_hd44780_i2c.h"

/**
 * Display clock
 *
 * @param show_header Show the header row
 */
void display_clock(bool show_header) {
    if (show_header) {
        lcd_puts(1, "==== Clock mode ====");
    }

    rtc_read_time();

    char line[LCD_COLS + 1];
    snprintf(line, sizeof(line), "Time: %02d:%02d:%02d", get_hours(), get_minutes(), get_seconds());
    lcd_puts(2, line);
}