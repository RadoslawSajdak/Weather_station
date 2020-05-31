/*
 * RTC library
 * Created: 04.2020
 * Author : Mateusz Kozyra
 * Purpose : Part of project made at AGH UST.
 * Description : This library contain function and definition for RTC (Real Time Clock) module 
 *					!!! TWI.h !!! library necessary
 * Features : RTC initialization, data and time set-function, data and time read, alarm set,
 *				alarm procedure
 */ 


#ifndef RTC_H_
#define RTC_H_

#define F_CPU 16000000UL//16 MHz
#include <util/delay.h>#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "TWI.h"					//we need TWI communication


/*******************************EXTERNAL DEVICE TWI ADDRESS***************/
#define RTC		 0x68
#define EXPANDER 0x20
/*************************************************************************/	
/*******************************RTC REGISTERS*****************************/
#define RTC_SEC		0X00
#define RTC_MIN		0X01
#define RTC_HOUR	0X02
#define RTC_H_24FORMAT		0X00	//instruction to set 24 hour format
#define RTC_H_12FORMAT		0X80	//instruction to set 12 hour format
#define RTC_DAY		0X03
#define RTC_DATE	0X04
#define RTC_MONTH	0X05
#define RTC_YEAR	0X06
#define RTC_TEMPERATURE_INT		0X11
#define RTC_TEMPERATURE_FRACT	0X12
#define RTC_CONTROL_REGISTER	0X0E
#define RTC_CR_DEFAULT	0x1C		//
#define RTC_CR_A2IE		0X02		//control register option
#define RTC_CR_A1IE		0X01		//
#define RTC_ALARM_VALUE 0x0A
/*************************************************************************/	
#define ALARM_SPEAKER_PIN		3
#define ALARM_SPEAKER_DDR		DDRC
/*******************************DEFINITION CUSTOM TYPE *******************/
typedef struct Time{
	uint8_t sec;
	uint8_t min;
	uint8_t h;
	};
	
typedef struct Date{
	uint8_t day;
	uint8_t month;
	uint8_t year;
	};
/*************************************************************************/	
/************************************************************************
 @brief Set RTC configuration - Alarm interrupt 
************************************************************************/
void rtc_init();

/************************************************************************
 @brief Read time data from RTC - it read 3 registers from start_point

 @param[in] time - structure pointer where we store time data (sec, min, h)
************************************************************************/
void rtc_read_time(struct Time *time, uint8_t start_point);

/************************************************************************
 @brief Set data in RTC registers - 3 registers from start_point

 @param[in] time - structure pointer where we have written data (sec, min, h)
************************************************************************/
void rtc_set_time(struct Time *time, uint8_t start_point);

/************************************************************************
 @brief Read date data from RTC - it read 3 registers

 @param[in] date - structure pointer where we store date data (day, month, year)
************************************************************************/
void rtc_read_date(struct Date *date);

/************************************************************************
 @brief Set data in RTC registers - 3 registers

 @param[in] date - structure pointer where we have written data (day, month, year)
************************************************************************/
void rtc_set_date(struct Date *date);

/************************************************************************
 @brief Enable alarm interrupt
************************************************************************/
void rtc_alarm_on();

/************************************************************************
 @brief Clear alarm interrupt flag
************************************************************************/
void rtc_alarm_clear();

/************************************************************************
 @brief Alarm procedure
************************************************************************/
void alarm();

#endif /* RTC_H_ */