#include <avr/interrupt.h>

#include "../board/board.h"

// app mode
volatile uint8_t app_mode = COMMON_MODE;

/**
 * Start debounce timer ISR
 */
static void start_debounce_timer(void) {
    TCCR0B = 0;
    TCCR0A = 0;
    TCNT0 = 0;
    TIFR0 |= (1 << TOV0);
    TIMSK0 |= (1 << TOIE0);
    TCCR0B = (1 << CS02) | (1 << CS00);
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

    // Btn still pressed
    if (!(PIND & (1 << PD3))) {
        if (app_mode != METRICS_SEND_MODE) {
            app_mode++;
        } else {
            app_mode = COMMON_MODE;
        }
    }

    // Clear pending btn flag
    EIFR |= (1 << INTF1);

    // Enable btn ISR again
    EIMSK |= (1 << INT1);
}

/**
 * Set ISR btn state on click
 */
ISR(INT1_vect) {
    // Disable btn ISR
    EIMSK &= ~(1 << INT1);

    start_debounce_timer();
}

/**
 * Allow button ISR
 */
void btn_mode_init(void) {
    // Set PD3 as input
    DDRD &= ~(1 << PD3);

    // Enable internal pull-up
    PORTD |= (1 << PD3);

    // ISR by falling edge
    EICRA |= (1 << ISC11);
    EICRA &= ~(1 << ISC10);

    // Clear pending btn flag
    EIFR |= (1 << INTF1);

    // Enable INT1 ISR
    EIMSK |= (1 << INT1);

    // Start global ISR
    sei();
}