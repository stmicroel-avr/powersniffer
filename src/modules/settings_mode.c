#include <string.h>
#include <util/delay.h>

#include "../board/board.h"
#include "../drivers/uart.h"
#include "../devices/lcd_hd44780_i2c.h"

/**
 * Run settings (HC-05 ble set name/pass)
 */
void settings_mode(void) {
    char response[21];

    lcd_clear();
    _delay_ms(100);
    lcd_puts(1, "Settings mode");

    // --- AT RESET ---
    lcd_puts(2, "Cmd: RESET");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, " ");

    uart_print("AT+ORGL\r\n");
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK");
    } else {
        lcd_puts(3, "Status: Failed(stop)");
        while (1);
    }
    _delay_ms(1500);

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

    // --- PSWD ---
    lcd_puts(2, "Cmd: AT+PSWD?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, "  ");

    uart_print("AT+PSWD?\r\n");
    if (uart_read_line(response, sizeof(response), 2000)) {
        lcd_puts(3, "Status: OK");
        lcd_puts(4, response);
    } else {
        lcd_puts(3, "Status: Failed");
    }
    uart_expect("OK", 1000);
    _delay_ms(1500);

    // --- PWD ---
    lcd_puts(2, "Cmd: AT+PWD?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, "  ");

    uart_print("AT+PWD?\r\n");
    if (uart_read_line(response, sizeof(response), 2000)) {
        lcd_puts(3, "Status: OK");
        lcd_puts(4, response);
    } else {
        lcd_puts(3, "Status: Failed");
    }
    uart_expect("OK", 1000);
    _delay_ms(1500);

    // --- UART (GET CURRENT BAUD) ---
    lcd_puts(2, "Cmd: AT+UART?");
    lcd_puts(3, "Wait response...");
    lcd_puts(4, "  ");

    uart_print("AT+UART?\r\n");
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

    // --- SET PASSWORD ---
    lcd_puts(2, "Cmd: SET PASSWORD  ");
    lcd_puts(3, "Wait response...   ");
    lcd_puts(4, " ");

    uart_print("AT+PSWD=6049\r\n");
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK");
    } else {
        lcd_puts(3, "Status: Failed");
    }
    _delay_ms(1500);

    // --- SET UART 38400 ---
    lcd_puts(2, "Cmd: SET UART");
    lcd_puts(3, "38400...");
    lcd_puts(4, " ");

    uart_print("AT+UART=38400,0,0\r\n");
    if (uart_expect("OK", 2000)) {
        lcd_puts(3, "Status: OK");
    } else {
        lcd_puts(3, "Status: Failed");
    }
    _delay_ms(1500);

    // --- DONE ---
    lcd_puts(2, "Settings done");
    lcd_puts(3, "Saved");
    lcd_puts(4, "Please reboot");

    while (1);
}