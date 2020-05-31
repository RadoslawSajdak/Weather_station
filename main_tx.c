/*
 * Weather station sensor
 * Created: 2020-04-17 20:21:02
 * Author : Sajdak Radoslaw
 * Description: Main file for sensor. Include radio and  DHT11 sensor
 * 
 */ 

#include <avr/io.h>
#include <avr/sleep.h>
#include "avr/interrupt.h"
#include "RadioRF24.h"
#include "DHT11.h"


uint8_t data[] = {0x25,0x89,0x99,0x56}; // Initialize with random data
uint8_t timer = TIME_PROTECT;

int main(void)
{
	/**	Interrupts init **/
	DDRD = 0;
	PORTD = 4;
	EICRA = (0 << ISC01) | (0 << ISC00);
	EIMSK = (1 << INT0);
	EIFR  = (1 << INTF1) | (1 << INTF0);
	
	TIMSK1 = (1<<TOIE1);
	TCNT1 = 0x00;
	TCCR1B |= (1 << CS12) | (1 << CS10);

	
	rf24_init(1,2,4);
	_delay_ms(5000);
	
	sei();

    while (1) 
    {
		
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
    }
	
	
	
}

ISR(INT0_vect)
	{
		/** Received question for data **/
		cli();
		PORTD = 4;
		DDRD = 0;
		_delay_ms(2000);
		EIFR  = (1 << INTF1) | (1 << INTF0);
		
		/** Change role to transmitter, measure data and send it **/
		rf24_init(0,2,4);
		dht11_read(data);
		
		while(!(rf24_get_status() & ((1 << TX_DS) | (1 << 4))))
		{
			
			_delay_ms(100);
			rf24_tx(data,4);
			_delay_ms(100);
		}
		
		/** Back to receiver role, reset timer and leave interrupt **/
		rf24_init(1,2,4);

		TCNT1 = 0;
		timer = TIME_PROTECT;
		sei();
		
		
	}
	
	ISR(TIMER1_OVF_vect)
	{
		cli();
		timer--;
		TCNT1 = 0;
		/** Too long without any data question. Force sending **/
		if (timer == 0)
		{
			timer = TIME_PROTECT;
			DDRD = 4;
			PORTD = 0;
			
		}
		sei();
	}