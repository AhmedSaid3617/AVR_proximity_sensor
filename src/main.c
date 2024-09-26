#define F_CPU 16000000

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "main.h"

#define LED PB0
#define TRIGGER PC4
#define ECHO PB1

uint32_t ticks = 0;
uint8_t distance_cm = 0xFF;
uint8_t echo_delay[2] = {0xf1, 0xf2};

int main()
{
    sei();

    DDRB |= 1 << LED;
    DDRD |= 1 << PD1;
    DDRD |= 1 << PD0;
    DDRC |= 1 << BUZZER;

    USART_Init();
    //timer_interrupt_init();
    Ultrasonic_init();
    
    while (1)
    {
        distance_cm = *((uint16_t*)echo_delay)/(58*2);
        US_send_pulse();
        _delay_ms(200);
        if (distance_cm <= 50)
        {
            PORTB |= 1<<LED;
            USART_Transmit(distance_cm);
        }
        else
        {
            PORTB &= ~(1<<LED);
            USART_Transmit(0xFF);
        }
        
        
    }
}

ISR(PCINT0_vect)
{
    // If rising edge.
    if (PINB & 1<<ECHO)
    {
        // Start the timer.
        TCCR1B |= 2;
    }
    
    // If falling edge.
    else
    {
        // Record the time then stop the counter.
        *((uint16_t*)(echo_delay)) = TCNT1;
        TCCR1B = 0;
        TCNT1 = 0;

        PCMSK1 &= ~(1<<PCINT11);    // Disable external interrupts.
    }
}

void Ultrasonic_init()
{
    TCNT1 = 0;              // Make sure the timer is cleared.
    DDRC |= 1 << TRIGGER;   // Trigger is output.
    DDRB &= ~(1 << ECHO);   // Echo is input.
    PCICR |= 1 << PCIE0;    // Enable external interrupt.
    PCMSK0 |= 1<<PCINT1;   // Unmask Echo pin interrupts.
}

void US_send_pulse()
{
    // Send pulse.
    PORTC &= ~(1<<TRIGGER);
    for (uint16_t i = 0; i < 10; i++)
    {
        /* code */
    }
    

    PORTC |= 1<<TRIGGER;
    for (uint16_t i = 0; i < 40; i++)
    {
        /* code */
    }

    PORTC &= ~(1<<TRIGGER);

    
}

void USART_Init()
{
    /*Set baud rate */
    UBRR0L = 103;
    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}
