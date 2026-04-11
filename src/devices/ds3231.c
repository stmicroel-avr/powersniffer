#include "ds3231.h"

#include "../board/board.h"
#include "../drivers/twi.h"

// Date time struct
struct {
    uint8_t y,M,d,h,m,s;
} time_t = {0,0,0,0,0,0};

/**
 * Convert BCD value to dec
 *
 * @param v value
 * @return
 */
static uint8_t bcd2dec(uint8_t v) {
    return ((v >> 4) * 10U) + (v & 0x0F);
}

/**
 * RTC Read time
 *
 * @return void
 */
void rtc_read_time(void) {
    uint8_t st;

    st = twi_start((DS3231_ADDR << 1) | 0);
    if (st != 0x18) {
        twi_stop();
        return;
    }

    st = twi_write(0x00);
    if (st != 0x28) {
        twi_stop();
        return;
    }

    st = twi_start((DS3231_ADDR << 1) | 1);
    if (st != 0x40) {
        twi_stop();
        return;
    }

    time_t.s = bcd2dec(twi_read_ack() & 0x7F);
    time_t.m = bcd2dec(twi_read_ack() & 0x7F);
    time_t.h = bcd2dec(twi_read_nack() & 0x3F);

    twi_stop();

    if (time_t.h == 0 && time_t.m == 0) {
        rtc_read_date();
    }
}

/**
 * RTC Read date
 */
void rtc_read_date(void) {
    uint8_t st;

    st = twi_start((DS3231_ADDR << 1) | 0);
    if (st != 0x18) {
        twi_stop();
        return;
    }

    st = twi_write(0x04);   // стартуем с регистра date
    if (st != 0x28) {
        twi_stop();
        return;
    }

    st = twi_start((DS3231_ADDR << 1) | 1);
    if (st != 0x40) {
        twi_stop();
        return;
    }

    twi_stop();

    time_t.d = bcd2dec(twi_read_ack() & 0x3F);
    time_t.M = bcd2dec(twi_read_ack() & 0x1F);
    time_t.y = bcd2dec(twi_read_nack());
}

/**
 * RTC Init date time
 *
 * @return
 */
void rtc_init(void) {
    rtc_read_date();
    rtc_read_time();
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

/**
 * Get day
 *
 * @return
 */
uint8_t get_day(void) {
    return time_t.d;
}

/**
 * Get month
 *
 * @return
 */
uint8_t get_month(void) {
    return time_t.M;
}

/**
 * Get year
 *
 * @return
 */
uint8_t get_year_from_2000(void) {
    return time_t.y;
}