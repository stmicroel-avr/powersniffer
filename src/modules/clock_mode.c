#include <stdbool.h>

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
}