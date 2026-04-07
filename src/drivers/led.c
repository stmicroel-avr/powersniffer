#include <stdint.h>
#include <util/delay.h>

#include "../board/board.h"

/**
 * Initialize port
 */
void led_init(void) {
    LED_DDR |= 1 << LED_PIN;
}

/**
 * Blink led
 *
 * @param num Number of blinks
 * @param mode Mode (infinite=2,short=1,long=any)
 */
void led_blink(uint8_t num, uint8_t mode) {
    uint8_t max = num * 2;

    uint8_t counter = 0;
    while (1) {
        LED_PORT ^= 1 << LED_PIN;

        // Infinite mode!
        if (mode == 2) {
            _delay_ms(LED_DELAY_SHORT);
        } else if (mode == 1) {
            _delay_ms(LED_DELAY_SHORT);
            counter++;
        } else {
            _delay_ms(LED_DELAY_LONG);
            counter++;
        }

        if (counter >= max) {
            break;
        }
    }
}
