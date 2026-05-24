#include "../drivers/twi.h"

#include "../board/board.h"

/**
 * Write 16-bit value to INA219 register.
 *
 * @param addr Device address
 * @param reg Register
 * @param value Value
 */
static void ina219_write_reg(uint8_t addr, uint8_t reg, uint16_t value) {
    twi_start((addr << 1) | 0);
    twi_write(reg);
    twi_write(value >> 8);
    twi_write(value & 0xFF);
    twi_stop();
}

/**
 * Read 16-bit value from INA219 register.
 *
 * @param addr Device address
 * @param reg Register
 * @return
 */
static uint16_t ina219_read_reg(uint8_t addr, uint8_t reg) {
    uint16_t value;

    twi_start((addr << 1) | 0);
    twi_write(reg);

    twi_start((addr << 1) | 1);
    value = (uint16_t)twi_read_ack() << 8;
    value |= twi_read_nack();
    twi_stop();

    return value;
}

/**
 * Initialize INA219 with the default configuration
 *
 * @param addr Device address
 * @param reg_calib_value Shunt resistance calibration value
 */
void ina219_init(uint8_t addr, uint16_t reg_calib_value) {
    // 32V range, ±320mV shunt, 12-bit ADC, continuous mode
    ina219_write_reg(addr, INA219_REG_CONFIG, 0x399F);
    // Calibration for 0.1 ohm, Current_LSB = 100uA
    ina219_write_reg(addr, INA219_REG_CALIB, reg_calib_value);
}

/**
 * Read bus voltage in volts.
 *
 * @param addr Device address
 * @return
 */
float ina219_read_bus_voltage(uint8_t addr) {
    uint16_t raw = ina219_read_reg(addr, INA219_REG_BUS);
    raw >>= 3;
    return raw * 0.004f;
}

/**
 * Read current in amperes.
 *
 * @param addr Device address
 * @return
 */
float ina219_read_current(uint8_t addr) {
    int16_t raw = (int16_t)ina219_read_reg(addr, INA219_REG_CURRENT);
    if (raw < 0) {
        return 0;
    }
    return raw * 0.0001f;
}