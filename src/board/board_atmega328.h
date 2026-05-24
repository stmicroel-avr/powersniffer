#pragma once

#include <avr/io.h>

// CPU CONF
#define F_CPU              8000000UL

// BUTTONS CONF
#define COMMON_MODE        1
#define CLOCK_MODE         2
#define METRICS_SEND_MODE      3

// I2C CONF
#define I2C_BUS_FREQ       100000L

// UART CONF
#define UART_BAUD          38400

// HC-05
#define HC05_NETWORK_NAME  "PowerSniffer"
#define HC05_NETWORK_PASS  "1111"

// SETTINGS MODE PIN
#define SETTINGS_DDR       DDRD
#define SETTINGS_GPIO      PD7
#define SETTING_PIN        PIND

// LED CONF
#define LED_PORT           PORTB
#define LED_DDR            DDRB
#define LED_PIN            PB0
#define LED_DELAY_SHORT    100
#define LED_DELAY_LONG     400
#define LED_MODE_INFINITE  2
#define LED_MODE_SHORT     1
#define LED_MODE_LONG      0

// LCD CONF
#define LCD_I2C_ADDR       0x20
#define LCD_COLS           20
#define LCD_ROWS           4

// INA219 ADDRESSES
#define INA219_ADDR_B1     0x40
#define INA219_ADDR_B2     0x45

// INA219 CONF
#define INA219_REG_CONFIG  0x00
#define INA219_REG_BUS     0x02
#define INA219_REG_CURRENT 0x04
#define INA219_REG_CALIB   0x05

// RTC
#define DS3231_ADDR        0x68

// 24c EEPROM
#define EXT_EEPROM_ADDR    0x50

// APP
#define REFRESH_RATE_MS    200
#define REFRESH_RTC_MS     950
#define RTC_START_UNIXTIME 946684800