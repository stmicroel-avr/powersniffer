#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize TWI in master mode.
 * @param scl_hz Desired SCL frequency in Hz.
 */
void twi_init(uint32_t scl_hz);

/**
 * @brief Send START and slave address.
 * @param addr_rw (addr << 1) | R/W bit.
 * @return 1 when done.
 */
uint8_t twi_start(uint8_t addr_rw);

/**
 * @brief Send STOP condition.
 */
void twi_stop(void);

/**
 * @brief Write one byte to bus.
 * @param data Byte to transmit.
 * @return 1 when done.
 */
uint8_t twi_write(uint8_t data);

/**
 * @brief Read byte with ACK.
 * @return Received byte.
 */
uint8_t twi_read_ack(void);

/**
 * @brief Read byte with NACK.
 * @return Received byte.
 */
uint8_t twi_read_nack(void);

/**
 * Check device ready state
 * @param addr I2C Address
 * @return
 */
bool twi_check_addr(uint8_t addr);