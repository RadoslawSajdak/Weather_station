/*
 * Weather station sensor - DHT11
 * Created: 2020-04-17 20:21:02
 * Author : Sajdak Radoslaw
 * Description: Custom library for DHT11 sensor
 */ 


#ifndef DHT11_H_
#define DHT11_H_

#define F_CPU 16000000UL
#include "avr/io.h"
#include <stdlib.h>
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


typedef struct Sensor{
	uint8_t temp_int;
	uint8_t temp_fractal;
	uint8_t humidity_int;
	uint8_t humidity_fractal;
};

/************************************************************************/
/* @brief	Initialize device to read analog input
/* 
/* @param[in]	uint8_t pointer to stored data
/************************************************************************/
void dht11_read(uint8_t * data);

/************************************************************************/
/* @brief	Data conversion to structure Sensor. It's need because DHT11
/*			returns 4 elements array with int data while MENU.h needs 	
/*			data as float
/*
/* @param[in]	pointer to array with data read from dht11_read function
/*				or 4 elements array with humidity_int, humidity_fractal,
/*				temp_int, temp_fractal in turn
/* @param[in]	pointer to structcture type Sensor. It will place here
/*				converted data in form friendly for MENU.h
/***********************************************************************/
void dht11_conversion(uint8_t * data, struct Sensor * converted);

#endif /* DHT11_H_ */