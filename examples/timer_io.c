#include "avr/io.h"

#include "main.h"


int main()
{
    DDRB |= 0b00100000; // Configuring PB6 as Output
    DDRB &= ~(1<<0); // Configuring PB0 as Input
    PORTB |= (1<<0); // Pull up resistor.
    
    while (1)
    {
        PORTB |= 0b00100000;
        timer_delay(1000);
        
        
        PORTB &= 0b11011111;
        timer_delay(1000);
        
    }
}

void timer_delay(uint32_t ms){
    uint32_t ticks = 0;

    TCCR1B |= 2; // Start counter.
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

void led_on(){
    PORTB |= 1<<6;
}

void led_off(){
    PORTB &= ~(1<<6);
}