#include "RTC.h"



void rtc_init(){
	twi_start();
	twi_write_byte((RTC<<1));
	twi_write_byte(RTC_CONTROL_REGISTER);
	twi_write_byte(RTC_CR_DEFAULT | RTC_CR_A2IE );			//alarm2 interrupt
	twi_stop();
	
	ALARM_SPEAKER_DDR|=(1<<ALARM_SPEAKER_PIN);				//speaker output
}

void rtc_read_time(struct Time *time, uint8_t start_point){

	twi_start();
	twi_write_byte((RTC<<1));
	twi_write_byte(start_point);
	
	twi_start();											//write starting point and start new transmission as receiver
	uint8_t buffer;											//buffer because data are written in specific way
	twi_write_byte(((RTC<<1)+1));
	buffer=twi_read_byte(1);
	time->sec=(buffer & 0x0f) + (((buffer & 0x70) >> 4) *10);	//convert data to int value
	buffer=twi_read_byte(1);
	time->min=(buffer & 0x0f) + (((buffer & 0x70) >> 4) *10);
	buffer=twi_read_byte(0);
	time->h=(buffer & 0x0f) + (((buffer & 0x30) >> 4) *10);
	twi_stop();

}

void rtc_set_time(struct Time *time, uint8_t start_point){
	twi_start();
	uint8_t buffer;
	twi_write_byte((RTC<<1));
	twi_write_byte(start_point);
	buffer=(((time->sec)/10) << 4) + ((time->sec)%10);
	twi_write_byte(buffer);
	buffer=(((time->min)/10) << 4) + ((time->min)%10);
	twi_write_byte(buffer);
	buffer=(((time->h)/10) << 4) + ((time->h)%10);
	twi_write_byte(buffer);
	twi_stop();
}

void rtc_read_date(struct Date *date){

	twi_start();
	twi_write_byte((RTC<<1));
	twi_write_byte(RTC_DATE);
	
	twi_start();
	uint8_t buffer;
	twi_write_byte(((RTC<<1)+1));
	buffer=twi_read_byte(1);
	date->day=(buffer & 0x0f) + (((buffer & 0x30) >> 4) *10);
	buffer=twi_read_byte(1);
	date->month=(buffer & 0x0f) + (((buffer & 0x10) >> 4) *10);
	buffer=twi_read_byte(0);
	date->year=(buffer & 0x0f) + (((buffer & 0xf0) >> 4) *10);
	twi_stop();
}

void rtc_set_date(struct Date *date){
	twi_start();
	uint8_t buffer;
	twi_write_byte((RTC<<1));
	twi_write_byte(RTC_DATE);
	buffer=(((date->day)/10) << 4) + ((date->day)%10);
	twi_write_byte(buffer);
	buffer=(((date->month)/10) << 4) + ((date->month)%10);
	twi_write_byte(buffer);
	buffer=(((date->year)/10) << 4) + ((date->year)%10);
	twi_write_byte(buffer);
	twi_stop();
}

void rtc_alarm_on(){
	cli();
	twi_start();
	twi_write_byte((RTC<<1));
	twi_write_byte(0x0D);					//0x0D alarm2 register
	twi_write_byte(0x81);					//0x81 value based on data sheet
	twi_stop();
	_delay_ms(2);
	sei();
}

void rtc_alarm_clear(){
	twi_start();
	twi_write_byte((RTC<<1));
	twi_write_byte(0x0F);					//0x0F register containing flags
	twi_write_byte(0x80);					//clear flag
	twi_stop();
	_delay_ms(2);
}

////////////////// INSIDE FUNCTIONS
void my_delay_us(int n) {		//we can't make delay with variable
	while(n--) {
		_delay_us(1);
	}
}


void tone(int Hz){				//play sound in certain frequency 
	int wait = 1000000/Hz;
	PORTC^=0x08;
	my_delay_us(wait);
}
///////////////////////////////////

void alarm(){
	for (int i=100; i<15000; i+=10) tone(i);

	for (int i=15000; i>100; i-=10) tone(i);

}