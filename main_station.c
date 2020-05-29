/*
 * MAIN code
 * Created: 04.2020
 * Author : Mateusz Kozyra
 * Purpose : Part of project made at AGH UST.
 * Description : Main code - all libraries required 
 * Features : Read and display (LCD display) data from RTC and temperature&humidity sensor, alarm-clock  
 */ 


#include <avr/io.h>
#include "LCD.h"
#include "TWI.h"
#include "RTC.h"
#include "MENU.h"
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

uint8_t state = 1;				//important variables
uint8_t substate = 1;

uint8_t data_outdoor[4] = {0x64,0x0B,0x53,0x11};
uint8_t data_indoor[4] = {0x64,0x0B,0x53,0x11};
uint8_t ret_code[MAX_BUFF];

void read_sensor();

int main(void)
{	
	
	init_UART();
	FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &str_uart;
	
	lcd_init();
	rtc_init();
	rf24_init(0,2,4);
	struct Time time;									//to hold RTC values
	struct Date date;
	
	PORTD=PORTD | (1<<PORTD2) | (1<<PORTD3);
	EIMSK |= 1<<INT0 | 1<<INT1;							// enable external interrupt 0
	MCUCR = (1<<ISC01 | 1<<ISC00);
	MCUCR = (1<<ISC10);
	sei();
	
	while (1){
		
		menu_interface_cleaning(&substate);
		while(state==1) menu_screen_time1(&state,&substate,&time,&date);	
		
		menu_interface_cleaning(&substate);
		while (state == 2) menu_screen_alarm2(&state, &substate, &time);
		
		menu_interface_cleaning(&substate);
		while (state == 3)
		{
			read_sensor();
			menu_screen_temperature3(&data_outdoor, &data_indoor);
		}
		while (state == 99)	menu_screen_alarm99(&state, &substate);
	}
}

void read_sensor()
{
	int transmition_limit = 10;
	rf24_init(0,2,4);
	printf("INIT AS TX\n\r");
	transmition_limit  = 5;
	while( !(rf24_get_status() & (1 << TX_DS)) && transmition_limit)
	{
		rf24_tx(data_outdoor,4);
		transmition_limit--;
		
	}
	rf24_reset();
	printf("SENT %d\n\r", (transmition_limit-100)*(-1));
	rf24_init(1,2,4);
	printf("INIT AS RX\n\r");
	rf24_rx(data_outdoor,4);
	printf("HUMIDITY: %d.%d    TEMP: %d.%d \n\r",data_outdoor[0], data_outdoor[1], data_outdoor[2], data_outdoor[3]);
	_delay_ms(500);
}
ISR(INT0_vect)					//alarm interruption
{
	EIMSK ^= 1<<INT0;
	substate=1;
	state=99;
}

ISR(INT1_vect)					//button interruption
{
	twi_start();
	uint8_t buffer;
	twi_write_byte(((EXPANDER<<1)+1));
	buffer=twi_read_byte(0);
	twi_stop();
	
	if(!(buffer & MENU_BUTTON_OK)) substate++;
	if(!(buffer & MENU_BUTTON_NEXT)) state++;
	if(!(buffer & MENU_BUTTON_BACK)) state--;
	
	if(state==4) state=1; else if(state==0) state=3;	//if out of menu range
}