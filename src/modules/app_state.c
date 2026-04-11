#include <avr/interrupt.h>

#include "../board/board.h"

// app mode
volatile uint8_t app_mode = COMMON_MODE;

/**
 * Start debounce timer ISR
 */
static void start_debounce_timer(void) {
    TCNT0 = 0;
    TCCR0B |= (1 << CS02) | (1 << CS00);
    TIMSK0 |= (1 << TOIE0);
}

/**
 * Stop debounce timer ISR
 */
void stop_debounce_timer(void) {
    TCCR0B = 0;
    TIMSK0 &= ~(1 << TOIE0);
}

/**
 * Timer overflow - check btn state
 */
ISR(TIMER0_OVF_vect) {
    stop_debounce_timer();

    // Btn still press
    if (!(PIND & (1 << PD3))) {
        if (app_mode != METRICS_SEND_MODE) {
            app_mode++;
        } else {
            app_mode = COMMON_MODE;
        }
    }

    // Enable btn ISR again
    EIMSK |= 1 << INT1;
}

/**
 * Set ISR btn state on click
 */
ISR(INT1_vect) {
    // disable rising ISR
    EIMSK &= ~(1 << INT1);
    start_debounce_timer();
}

/**
* Allow button ISR
 */
void btn_mode_init(void) {
    // int pd3
    DDRD &= ~(1 << PD3);

    // isr by rising edge
    EICRA |= 1 << ISC11;   // ISC11 = 1
    EICRA &= ~(1 << ISC10);  // ISC10 = 0
    EIMSK |= 1 << INT1;

    // Start ISR
    sei();
}