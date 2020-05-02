/*
 * DHT11.h
 *
 * Created: 2020-04-24 01:34:13
 *  Author: Sajdak Rados³aw
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#include "avr/io.h"
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>


#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define DHT11_NUM	0
#define DHT11_PIN	PINB
#define DHT11_PORT	PORTB
#define DHT11_DDR	DDRB

#define MAX_TIMINGS	85

/************************************************************************/
/* @brief	Initialize device to read analog input
/************************************************************************/
void dht11_read(uint8_t * data);



#endif /* DHT11_H_ */