#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t g_ms = 0;

/**
 * Initialize ms timer
 */
void timer_init(void) {
    TCCR0A = 1 << WGM01;
    OCR0A  = 124;
    TIMSK0 = 1 << OCIE0A;
    TCCR0B = (1 << CS01) | (1 << CS00);
    sei();
}

/**
 * Get ms timer value
 *
 * @return
 */
uint32_t ms_now(void) {
    uint32_t t;
    uint8_t s = SREG;
    cli();
    t = g_ms;
    SREG = s;
    return t;
}

ISR(TIMER0_COMPA_vect) {
    g_ms++;
}