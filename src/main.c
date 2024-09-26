#define F_CPU 16000000

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "main.h"

uint32_t ticks = 0;
uint8_t distance = 0xFF;

int main()
{
    sei();

    /* DDRB |= 0b00100000; // Configuring PB6 as Output
    DDRB &= ~(1<<0); // Configuring PB0 as Input
    PORTB |= (1<<0); // Pull up resistor.
    PORTB |= 0b00100000; */

    DDRB |= 1 << PB0;
    DDRD |= 1 << PD1;
    DDRD |= 1 << PD0;
    // PORTB |= 1<<PB0;
    timer_interrupt_init();
    USART_Init();
    while (1)
    {

    }
}

ISR(TIMER1_COMPA_vect)
{
    if ((ticks % (distance *10)) == 0)
    {
        PORTB ^= 1;
        USART_Transmit(distance);
    }

    ticks++;
}

ISR(USART_RX_vect)
{
    cli();
    USART_Receive();
    sei();
}

void USART_Init()
{
    /*Set baud rate */
    UBRR0L = 103;
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    // RX interrupts.
    UCSR0B |= 1 << RXCIE0;
}

unsigned int USART_Receive(void)
{
    unsigned char status, resh, resl;
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get status and 9th bit, then data */
    /* from buffer */
    // resh = UCSR0B;

    // Get distance.
    distance = UDR0;

    /* Filter the 9th bit, then return */
    /* resh = (resh >> 1) & 0x01;
    return ((resh << 8) | resl); */
    return 1;
}

void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void timer_interrupt_init()
{
    OCR1A = 2000;         // Initialize the timer to interrupt every 1 ms.
    TIMSK1 |= 1 << OCIE1A; // Enable interrupts from output compare channel A.
    TCCR1B |= 1 << WGM12;  // CTC
    TCCR1B |= 2; // Start counter with prescaler = 8.
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