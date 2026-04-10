#include <stdbool.h>

#include "../board/board.h"
#include "../drivers/twi.h"

/**
 * Time struct
 */
struct {
    uint8_t h,m,s;
} time_t = {0,0,0};

/**
 * Convert BCD value to dec
 * @param v value
 * @return
 */
static uint8_t bcd2dec(uint8_t v) {
    return ((v >> 4) * 10U) + (v & 0x0F);
}

/**
 * RTC Read time
 *
 * @return bool
 */
bool rtc_read_time(void) {
    uint8_t st;

    st = twi_start((DS3231_ADDR << 1) | 0);
    if (st != 0x18) {
        twi_stop();
        return false;
    }

    st = twi_write(0x00);
    if (st != 0x28) {
        twi_stop();
        return false;
    }

    st = twi_start((DS3231_ADDR << 1) | 1);
    if (st != 0x40) {
        twi_stop();
        return false;
    }

    time_t.s = bcd2dec(twi_read_ack() & 0x7F);
    time_t.m = bcd2dec(twi_read_ack() & 0x7F);
    time_t.h = bcd2dec(twi_read_nack() & 0x3F);

    twi_stop();
    return true;
}

/**
 * Get hours
 *
 * @return
 */
uint8_t get_hours(void) {
    return time_t.h;
}

/**
 * Get minutes
 *
 * @return
 */
uint8_t get_minutes(void) {
    return time_t.m;
}

/**
 * Get seconds
 *
 * @return
 */
uint8_t get_seconds(void) {
    return time_t.s;
}