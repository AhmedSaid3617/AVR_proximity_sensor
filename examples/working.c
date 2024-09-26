#define F_CPU 8000000

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "main.h"

uint32_t ticks = 0;

int main()
{

    /* DDRB |= 0b00100000; // Configuring PB6 as Output
    DDRB &= ~(1<<0); // Configuring PB0 as Input
    PORTB |= (1<<0); // Pull up resistor.
    PORTB |= 0b00100000; */

    DDRB |= 1 << PB0;
    // PORTB |= 1<<PB0;
    timer_interrupt_init();
    sei();

    while (1)
    {
        /* PORTB |= 1<<PB0;
        _delay_ms(1000);


        PORTB &= ~(1<<PB0);
        _delay_ms(1000); */
    }
}

ISR(TIMER1_COMPA_vect)
{
    // TCNT1=0xE17B;
    // PORTB ^= 0b00100000;
    // TCNT0 = 0;
    /* PORTB ^= 1;
    if (ticks%500)
    {
        PORTB ^= 1;
    }

    ticks++; */
    PORTB ^= 1;
}

void timer_interrupt_init()
{
    OCR1A = 0xFFFF;          // Initialize the timer to interrupt every 1 ms.
    TIMSK1 |= 1 << OCIE1A; // Enable interrupts from output compare channel A.
    TCCR1B |= 1 << WGM12;
    TCCR1B |= 2; // Start counter with prescaler = 8.

    // TIMSK1 |= 1<<TOIE1;
    // TCNT1 = 0xE17B;
    // TIFR1 |= 0x01;
    // TCCR1B |= 3;
}

void timer_delay(uint32_t ms)
{
    uint32_t ticks = 0;

    TCCR1B |= 2; // Start counter with prescaler = 8.
    while (ticks < ms)
    {
        while (TCNT1 < 1000)
        {
        }
        TCNT1 = 0;
        ticks++;
    }

    TCCR1B = 0; // Stop counter.
}

void led_on()
{
    PORTB |= 1 << 6;
}

void led_off()
{
    PORTB &= ~(1 << 6);
}