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


uint8_t state = 1;				//important variables
uint8_t substate = 1;

uint8_t data_outdoor[4] = {0x64,0x0B,0x53,0x11};
uint8_t data_indoor[4] = {0x64,0x0B,0x53,0x11};
uint8_t ret_code[MAX_BUFF];

void read_sensor();

int main(void)
{	
	lcd_init();
	rtc_init();
	rf24_init(0,2,4);
	
	struct Time time;									//to hold RTC values
	struct Date date;
	struct Sensor inside_sensor;
	struct Sensor outside_sensor;
		
	//** Initialize data for sensors **//
	read_sensor();
	dht11_conversion(data_outdoor,&outside_sensor);
	dht11_read(data_indoor);
	dht11_conversion(data_indoor, &inside_sensor);
		
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
			menu_screen_temperature3(&state, &substate, &inside_sensor, &outside_sensor);
			
			int delay_multipier = 100; // n * 100ms
			read_sensor();
			dht11_conversion(data_outdoor,&outside_sensor);
			dht11_read(data_indoor);
			dht11_conversion(data_indoor, &inside_sensor);
			menu_screen_temperature3(&state, &substate, &inside_sensor, &outside_sensor);
			
			sei();
			while((delay_multipier > 0) & (state == 3) )
			{	
				
				_delay_ms(100);
				delay_multipier--;
				
			}
			cli();
		}
		while (state == 99)	menu_screen_alarm99(&state, &substate);
	}
}

void read_sensor()
{
	
	int transmition_limit = 10;
	rf24_init(0,2,4);
	transmition_limit  = 5;
	while( !(rf24_get_status() & (1 << TX_DS)) && transmition_limit )
	{
		rf24_tx(data_outdoor,4);
		transmition_limit--;
		
	}
	rf24_reset();
	rf24_init(1,2,4);
	rf24_rx(data_outdoor,4, &state);
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