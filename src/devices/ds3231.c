#include "../board/board.h"
#include "../drivers/twi.h"

// Date time struct
static struct {
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
 * RTC Read date
 */
void rtc_read_date(void) {
    uint8_t st;
    uint8_t raw_d, raw_M, raw_y;

    st = twi_start((DS3231_ADDR << 1) | 0);
    if (st != 0x18) { twi_stop(); return; }

    st = twi_write(0x04);
    if (st != 0x28) { twi_stop(); return; }

    st = twi_start((DS3231_ADDR << 1) | 1);
    if (st != 0x40) { twi_stop(); return; }

    raw_d = twi_read_ack();
    raw_M = twi_read_ack();
    raw_y = twi_read_nack();

    twi_stop();

    time_t.d = bcd2dec(raw_d & 0x3F);
    time_t.M = bcd2dec(raw_M & 0x1F);
    time_t.y = bcd2dec(raw_y);
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

/**
 * Get unixtime
 *
 * @return
 */
uint32_t get_unixtime(void) {
    static const uint8_t mdays[] = {
        31,28,31,30,31,30,31,31,30,31,30,31
    };

    uint32_t days = 0;
    uint16_t year = 2000 + time_t.y;

    for (uint8_t i = 0; i < time_t.y; i++) {
        uint16_t yr = 2000 + i;
        days += 365;

        if ((yr % 4 == 0 && yr % 100 != 0) || (yr % 400 == 0)) {
            days += 1;
        }
    }

    for (uint8_t i = 1; i < time_t.M; i++) {
        days += mdays[i - 1];
        if (i == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                days += 1;
            }
        }
    }

    days += (time_t.d - 1);

    uint32_t t = days * 86400UL;
    t += time_t.h * 3600UL;
    t += time_t.m * 60UL;
    t += time_t.s;

    return t + 946684800UL;
}