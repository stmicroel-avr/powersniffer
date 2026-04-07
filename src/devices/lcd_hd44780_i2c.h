#pragma once

/**
 * Init display
 */
void lcd_init(void);

/**
 * Clear display
 */
void lcd_clear(void);

/**
 * Print char
 * @param s
 */
void lcd_putc(char s);

/**
 * Display string
 *
 * @param row Row number
 * @param s String buffer
 */
void lcd_puts(uint8_t row, const char *s);