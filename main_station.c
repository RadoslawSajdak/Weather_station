/*
 * MAIN code
 * Created: 04.2020
 * Author : Mateusz Kozyra
 * Purpose : Part of project made at AGH UST.
 * Description : Main code - all libraries required 
 * Features : Read and display (LCD display) data from RTC and temperature&humidity sensor, alarm-clock  
 */ 



#include "LCD.h"
#include "TWI.h"
#include "RTC.h"
#include "MENU.h"

uint8_t state = 1;				//important variables
uint8_t substate = 1;

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

int main(void)
{	
	lcd_init();
	rtc_init();	
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
		while (state == 3) menu_screen_temperature3(&state, &substate);
	
		while (state == 99)	menu_screen_alarm99(&state, &substate);
	}
}