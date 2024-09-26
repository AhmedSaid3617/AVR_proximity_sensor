#ifndef MAIN_H
#define MAIN_H

void led_on();
void led_off();
void timer_delay(uint32_t ms);
void timer_interrupt_init();
void USART_Init();
void USART_Transmit(unsigned char data);
unsigned int USART_Receive(void);

#endif