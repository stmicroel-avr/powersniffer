#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define TWI_STATUS_MASK 0xF8

/**
 * Initialize TWI hardware.
 */
void twi_init(uint32_t scl_hz) {
    TWSR = 0x00; // prescaler = 1
    TWBR = (uint8_t)(((F_CPU / scl_hz) - 16) / 2);
    TWCR = (1 << TWEN);
}

/**
 * Generate START and send address.
 * Returns TWI status code.
 */
uint8_t twi_start(uint8_t addr_rw) {
    // Send START
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    uint8_t status = TWSR & TWI_STATUS_MASK;
    if (status != 0x08 && status != 0x10) {
        return status;
    }

    // Send SLA+R/W
    TWDR = addr_rw;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    status = TWSR & TWI_STATUS_MASK;
    return status;
}

/**
 * Generate STOP condition.
 */
void twi_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

/**
 * Transmit one byte.
 * Returns TWI status code.
 */
uint8_t twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    return (TWSR & TWI_STATUS_MASK);
}

/**
 * Read byte and send ACK.
 */
uint8_t twi_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

/**
 * Read byte and send NACK.
 */
uint8_t twi_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

/**
 * Check device ready state
 * @param addr I2C Address
 * @return
 */
bool twi_check_addr(uint8_t addr) {
    uint8_t st = twi_start((addr << 1) | 0);
    twi_stop();
    return (st == 0x18);
}