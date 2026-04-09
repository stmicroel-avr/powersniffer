#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t g_millis2 = 0;

/**
 * Init ms timer
 */
void init_ms_timer(void) {
    // stop timer(setup)
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;

    // CTC reset by interrupt
    TCCR2A |= (1 << WGM21);

    // 1ms config
    OCR2A = 124;

    // Interrupt on equal
    TIMSK2 |= (1 << OCIE2A);

    // Start timer
    TCCR2B |= (1 << CS22);
}

/**
 * Global interrupt
 */
ISR(TIMER2_COMPA_vect) {
    g_millis2++;
}

/**
 * Get ISR value
 * @return
 */
uint32_t get_ms(void) {
    uint32_t ms;

    // Чтобы безопасно читать 32-битную переменную,
    // временно запрещаем прерывания
    uint8_t sreg = SREG;
    cli();
    ms = g_millis2;
    SREG = sreg;

    return ms;
}