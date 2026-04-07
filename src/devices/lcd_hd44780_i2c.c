#include <stdint.h>
#include <util/delay.h>

#include "../board/board.h"
#include "../drivers/twi.h"

#define PIN_RS 0
#define PIN_E  1
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7

static uint8_t pcf_state = 0;

static uint8_t pcf8574_write(uint8_t data) {
    uint8_t st;

    st = twi_start((LCD_I2C_ADDR << 1) | 0);
    if (st != 0x18) {
        twi_stop();
        return st;
    }

    st = twi_write(data);
    twi_stop();
    return st;
}

/**
 * Apply current expander state
 */
static void lcd_expander_apply(void) {
    (void)pcf8574_write(pcf_state);
    _delay_us(20);
}

/**
 * Pulse E line
 */
static void pulse_e(void) {
    pcf_state |= (1 << PIN_E);
    lcd_expander_apply();
    _delay_us(2);

    pcf_state &= ~(1 << PIN_E);
    lcd_expander_apply();
    _delay_us(50);
}

/**
 * Write 4 bits to LCD
 *
 * @param n nibble (0..15)
 */
static void write4(uint8_t n) {
    pcf_state &= ~(
        (1 << PIN_D4) |
        (1 << PIN_D5) |
        (1 << PIN_D6) |
        (1 << PIN_D7)
    );

    if (n & 0x01) pcf_state |= (1 << PIN_D4);
    if (n & 0x02) pcf_state |= (1 << PIN_D5);
    if (n & 0x04) pcf_state |= (1 << PIN_D6);
    if (n & 0x08) pcf_state |= (1 << PIN_D7);

    lcd_expander_apply();
    pulse_e();
}

/**
 * Write 8 bits in 4-bit mode
 *
 * @param b Data byte
 * @param rs RS bit value
 */
static void send8(uint8_t b, uint8_t rs) {
    if (rs) {
        pcf_state |= (1 << PIN_RS);
    } else {
        pcf_state &= ~(1 << PIN_RS);
    }

    write4((b >> 4) & 0x0F);
    write4(b & 0x0F);

    if (b == 0x01 || b == 0x02) {
        _delay_ms(2);
    } else {
        _delay_us(50);
    }
}

/**
 * Send command byte
 *
 * @param c Command byte
 */
static void lcd_cmd(uint8_t c) {
    send8(c, 0);
}

/**
 * Set cursor position
 *
 * @param x Col position
 * @param y Row position
 */
static void lcd_gotoxy(uint8_t x, uint8_t y) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_cmd(0x80 | (row_offsets[y] + x));
}

/**
 * Map char to LCD-safe printable ASCII.
 * Everything outside basic ASCII becomes space.
 */
static uint8_t lcd_sanitize_char(char c) {
    uint8_t u = (uint8_t)c;

    if (u >= 32 && u <= 126) {
        return u;
    }

    return ' ';
}

/**
 * Display symbol
 *
 * @param s Char
 */
void lcd_putc(uint8_t s) {
    send8(s, 1);
}

/**
 * Clear display
 */
void lcd_clear(void) {
    lcd_cmd(0x01);
    _delay_ms(2);
}

/**
 * Initialize display
 *
 * NOTE:
 * twi_init(...) must be called before lcd_init().
 */
void lcd_init(void) {
    _delay_ms(50);

    /*
     * Initial state:
     * RS/E = 0
     * D4..D7 = 0
     */
    pcf_state = 0;
    lcd_expander_apply();
    _delay_ms(20);

    /* Switch LCD to 4-bit mode */
    write4(0x03); _delay_ms(5);
    write4(0x03); _delay_us(150);
    write4(0x03); _delay_us(150);
    write4(0x02); _delay_us(150);

    lcd_cmd(0x28); // 4-bit, 2-line, 5x8 font
    lcd_cmd(0x08); // display OFF during setup
    lcd_cmd(0x01); // clear
    _delay_ms(2);
    lcd_cmd(0x06); // entry mode: increment
    lcd_cmd(0x0C); // display ON, cursor OFF, blink OFF
}

/**
 * Display string and pad row with spaces to full width
 *
 * @param row Row number (1..LCD_ROWS)
 * @param s String buffer
 */
void lcd_puts(uint8_t row, const char *s) {
    if (row == 0 || row > LCD_ROWS) {
        return;
    }

    const uint8_t y = row - 1;
    lcd_gotoxy(0, y);

    uint8_t i = 0;

    while (i < LCD_COLS && s[i] != '\0') {
        lcd_putc(lcd_sanitize_char(s[i]));
        i++;
    }

    while (i < LCD_COLS) {
        lcd_putc(' ');
        i++;
    }
}