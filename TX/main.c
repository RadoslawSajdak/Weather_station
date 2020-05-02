/*
 * WeatherStation.c
 *
 * Created: 2020-04-17 20:21:02
 * Author : sajda
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"
#include "RadioRF24.h"
#include "DHT11.h"
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

uint8_t data[] = {0x25,0x89,0x99,0x56};
uint8_t ret_code[MAX_BUFF];
uint8_t timer = TIME_PROTECT;
int main(void)
{
	init_UART();
	FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &str_uart;
	
	//while(1){
	//	dht11_read(data);
	//	printf("Res: %d.%d  %d.%d\n\r",data[0],data[1],data[2],data[3]);
	//	_delay_ms(3000);
	//}
	
	DDRD = 0;
	PORTD = 4;
	EICRA = (0 << ISC01) | (0 << ISC00);
	EIMSK = (1 << INT0);
	EIFR  = (1 << INTF1) | (1 << INTF0);
	
	TIMSK1=(1<<TOIE1);
	TCNT1=0x00;
	TCCR1B |= (1 << CS12) | (1 << CS10);

	
	uint8_t code;
	code = rf24_get_status();
	printf("STATUS: %x\n\r", code);
	rf24_init(1,2,4);
	_delay_ms(5000);
	
	sei();
	rf24_get_register(CONFIG,2, ret_code);
	printf("Ret data: %x %x\n\r", ret_code[0],ret_code[1] );
	code = rf24_get_status();
	printf("STATUS: %x\n\r", code);
	//while(1)
	//{	
	//dht11_read(data);
	//rf24_tx(data, 4);
	//_delay_ms(1000);
	//}
    /* Replace with your application code */
    while (1) 
    {
		asm("nop");
    }
	
	
	
}

ISR(INT0_vect)
	{
		cli();
		PORTD = 4;
		DDRD = 0;
		printf("INTERRUPT!!!\n\r");
		_delay_ms(2000);
		EIFR  = (1 << INTF1) | (1 << INTF0);
		
		rf24_init(0,2,4);
		int i = 100;
		printf("test a_%d\n\r",(i-100)*(-1));
		dht11_read(data);
		while(!(rf24_get_status() & ((1 << TX_DS) | (1 << 4))))
		{
			
			printf("test b_%d\n\r",(i-100)*(-1));
			_delay_ms(100);
			printf("test c_%d\n\r",(i-100)*(-1));
			rf24_tx(data,4);
			_delay_ms(100);
			printf("stat: %x\n\r", rf24_get_status());
			printf("test d_%d\n\r",(i-100)*(-1));
			i--;
		}
		_delay_ms(100);
		printf("test e_%d\n\r",(i-100)*(-1));
		rf24_init(1,2,4);
		printf("INT DONE EIFR:%d\n\r",EIFR);
		TCNT1 = 0;
		
		timer = TIME_PROTECT;
		sei();
		
		
	}
	
	ISR(TIMER1_OVF_vect)
	{
		cli();
		
		printf("\n\rClock OVF %d\n\n\r", (timer - 5)*(-1));
		timer--;
		TCNT1 = 0;
		if (timer == 0)
		{
			timer = TIME_PROTECT;
			DDRD = 4;
			PORTD = 0;
			
		}
		sei();
	}