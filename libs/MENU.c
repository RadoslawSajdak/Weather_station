/*
 * MENU.c
 *
 * Created: 03.05.2020 00:32:36
 *  Author: Acer
 */ 
#include "MENU.h"

void menu_screen_time1(uint8_t *state, uint8_t *substate, struct Time *time, struct Date *date){
	_delay_ms(100);
	cli();
	lcd_set_position(0,4);
	
	rtc_read_time(time,RTC_SEC);
	_delay_ms(10);
	lcd_write_float(time->h);
	lcd_write_text(":");
	lcd_write_float(time->min);
	lcd_write_text(":");
	lcd_write_float(time->sec);
	
	lcd_set_position(1,3);
	rtc_read_date(date);
	_delay_ms(10);
	lcd_write_float(date->day);
	lcd_write_text(".");
	lcd_write_float(date->month);
	lcd_write_text(".20");
	lcd_write_float(date->year);
	sei();
}

void menu_screen_alarm2(uint8_t *state, uint8_t *substate, struct Time *time){
	cli();
	lcd_set_position(0,6);
	lcd_write_text("ALARM");
	_delay_ms(100);
	
	lcd_set_position(1,6);
	rtc_read_time(time,RTC_ALARM_VALUE);
	_delay_ms(10);
	lcd_write_float(time->h);
	lcd_write_text(":");
	lcd_write_float(time->min);
	sei();
	
	while (*substate!=1){			//sub menu	-alarm changing
		
		cli();
		lcd_set_position(1,6);
		rtc_read_time(time,RTC_ALARM_VALUE);
		
		lcd_write_float(time->h);
		lcd_write_text(":");
		lcd_write_float(time->min);
		if (*substate==2){		//set minutes
			lcd_set_position(0,6);
			lcd_write_text("   MM");
			sei();
			while (*state==2 && *substate==2)	{_delay_ms(1);}		//wait for interaction
			cli();
			if (*state==3) (time->min)++; else if (*state==1) (time->min)--;	//detect option
			if (time->min==60) time->min=0; else if (time->min==255) time->min=59;
		}
		
		if (*substate==3){		//set hours
			lcd_set_position(0,6);
			lcd_write_text("HH    ");
			sei();
			while (*state==2 && *substate==3)	{_delay_ms(1);}		//wait for interaction
			cli();
			if (*state==3) (time->h)++; else if (*state==1) (time->h)--;	//detect option
			if (time->h==24) time->h=0; else if (time->h==255) time->h=23;
		}
		if (*substate==4) *substate=1;		//end configuration
		rtc_set_time(time, RTC_ALARM_VALUE);
		*state=2;			//stay on this screen
		
	}
	rtc_alarm_on();
}

void menu_screen_temperature3(uint8_t *state, uint8_t *substate){ 
	cli();
	
	lcd_display_samples(state[0],state[2],substate[0],substate[2]);
	_delay_ms(100);
	sei();
}

void menu_interface_cleaning(uint8_t *substate){
	lcd_write_instruction(LCD_DISPLAY_CLEAR);
	_delay_ms(20);
	*substate=1;		//default substate
}


void menu_screen_alarm99(uint8_t *state, uint8_t *substate){
	cli();
	lcd_write_instruction(LCD_DISPLAY_CLEAR);
	_delay_ms(10);
	lcd_set_position(0,5);
	lcd_write_text("!ALARM!");
	lcd_set_position(1,1);
	lcd_write_text("<ok to cancel>");
	sei();
	while (*substate==1) {
		alarm();
	}
	cli();
	rtc_alarm_clear();
	*state = 2;
	*substate=1;
	lcd_write_instruction(LCD_DISPLAY_CLEAR);
	_delay_ms(10);
	EIMSK |= 1<<INT0;
}

