#include <avr/io.h>
#include <util/delay.h>

#include "../board/board.h"
#include "uart.h"

/**
 * Initialize UART hardware.
 *
 * @param baud Baud speed rate
 */
void uart_init(uint32_t baud) {
    uint16_t ubrr = (F_CPU / 16 / baud) - 1;

    // Set baud rate
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable RX and TX
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * Transmit one byte.
 *
 * @param data Data byte
 */
void uart_tx(uint8_t data) {
    // Wait until transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer, sends the byte
    UDR0 = data;
}

/**
 * Transmit null-terminated string.
 *
 * @param str String
 */
void uart_print(const char *str) {
    while (*str) {
        uart_tx((uint8_t)*str++);
    }
}

/**
 * Check if data is available.
 */
uint8_t uart_available(void) {
    return (UCSR0A & (1 << RXC0)) ? 1 : 0;
}

/**
 * Receive one byte (blocking).
 */
uint8_t uart_rx(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data
    return UDR0;
}

/**
 * Expect string from uart
 *
 * @param expected String
 * @param timeout_ms Timeout
 * @return
 */
uint8_t uart_expect(const char *expected, uint16_t timeout_ms) {
    uint8_t i = 0;
    uint16_t elapsed = 0;

    while (elapsed < timeout_ms) {
        if (UCSR0A & (1 << RXC0)) {
            char c = UDR0;

            if (c == expected[i]) {
                i++;
                if (expected[i] == '\0') {
                    return 1;
                }
            } else {
                i = 0;
            }
        } else {
            _delay_ms(1);
            elapsed++;
        }
    }

    return 0;
}

/**
 * Read one UART line into buffer.
 */
uint8_t uart_read_line(char *buffer, uint8_t buffer_size, uint16_t timeout_ms) {
    uint16_t elapsed = 0;
    uint8_t i = 0;

    while (elapsed < timeout_ms) {
        if (uart_available()) {
            char c = uart_rx();

            if (c == '\r') {
                continue;
            }

            if (c == '\n') {
                if (i == 0) {
                    continue;
                }

                buffer[i] = '\0';
                return 1;
            }

            if (i < (buffer_size - 1)) {
                buffer[i++] = c;
            }
        } else {
            _delay_ms(1);
            elapsed++;
        }
    }

    buffer[i] = '\0';
    return 0;
}