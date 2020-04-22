/*
 * WeatherStation.c
 *
 * Created: 2020-04-17 20:21:02
 * Author : sajda
 */ 

#include <avr/io.h>
#include "RadioRF24.h"

void init_UART(void)
{
  //Set baud rate 115200
  UBRR0H = 0;
  UBRR0L = 16;
  //Enable receiver and transmitter
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  //Double Clock Speed
  UCSR0A = (1<<U2X0);
  // Set frame format: 8data, 1stop bit
  UCSR0C = (3<<UCSZ00); //(1<<USBS0)|
}
int uart_putchar(char c, FILE *stream){
  //wait until buffer empty
  while ( !( UCSR0A & (1<<UDRE0)) );
  //Put data into buffer
  UDR0 = c;
  
  return 0;
}

uint8_t data[4]; //= {0x64,0x0B,0x53,0x11};
uint8_t ret_code[MAX_BUFF];
int main(void)
{
	init_UART();
	FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &str_uart;
	
	rf24_init(3,2,4);
	while(1)
	{	
	rf24_rx(data,4);
	printf("RESULT: %2X %2X %2X %2X  | STATUS: %x\n\r",data[0], data[1], data[2], data[3], rf24_get_status());
	_delay_ms(1000);
	}
    /* Replace with your application code */
    while (1) 
    {
		asm("nop");
    }
}

