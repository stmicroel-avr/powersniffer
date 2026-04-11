#pragma once

#include <stdint.h>

/**
 * Configures UART0 for asynchronous mode, 8 data bits, no parity, 1 stop bit (8N1).
 *
 * @param baud Desired baud rate (e.g. 9600, 38400)
 */
void uart_init(uint32_t baud);

/**
 * Waits until transmit buffer is ready, then writes data to UDR0.
 *
 * @param data Byte to send
 */
void uart_tx(uint8_t data);

/**
 * Sends characters one by one until '\0' is reached.
 *
 * @param str Pointer to null-terminated string
 */
void uart_print(const char *str);

/**
 * @return 1 if data is available, 0 otherwise
 */
uint8_t uart_available(void);

/**
 * Waits until data is available, then returns it.
 *
 * @return Received byte
 */
uint8_t uart_rx(void);

/**
 * Expect string from uart
 *
 * @param expected String
 * @param timeout_ms Timeout
 * @return
 */
uint8_t uart_expect(const char *expected, uint16_t timeout_ms);

/**
 * Read one UART line into buffer
 *
 * @param buffer Buffer
 * @param buffer_size Size of buffer
 * @param timeout_ms Timeout in ms
 * @return
 */
uint8_t uart_read_line(char *buffer, uint8_t buffer_size, uint16_t timeout_ms);