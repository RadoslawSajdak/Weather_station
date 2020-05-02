/*
 * nRF24L01 library
 *
 * 
 * 17.04.2020 Rados³aw Sajdak
 */ 
#ifndef RADIORF24_H_
#define RADIORF24_H_

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


#define CE		1
#define SCK		5
#define MISO	4		//Port B pins
#define MOSI	3
#define CSN		2
#define RF_PORT PORTB

#define CONFIG		0x00
#define EN_AA		0x01
#define EN_RX		0x02
#define SETUP_AW	0x03
#define SETUP_RETR	0x04
#define CHANNEL		0x05
#define RF_SETUP	0x06
#define STATUS		0x07
#define RX0_ADDR	0x0A
#define RX1_ADDR	0x0B
#define TX_ADDR		0x10
#define RX_PW_P0	0x11
#define RX_PW_P1	0x12
#define FIFO_STATUS	0x17
#define R_REGISTER	0x00
#define W_REGISTER  0x20
#define REG_MASK	0x1F
#define TX_PAYLOAD	0xA0
#define	RX_PAYLOAD	0x61
#define FLUSH_TX	0xE1
#define FLUSH_RX	0xE2

#define RF24_CONFIG 8
#define TX_DS		5
#define RX_DR		6
#define RF_ADDR_LEN 5	//must be 3-5
#define MAX_BUFF    32
#define TIME_PROTECT 5


/************************************************************************/
/* @brief Init function for radio as receiver
/*
/* @param[in] 0 for transmitter role, else for receiver role
/*
/* @return true on success
/************************************************************************/
void rf24_init(uint8_t role, uint8_t channel, uint8_t payload);

/************************************************************************/
/*	@brief function for pushing 8 bit to specified address through SPI
/*
/*	@param[in] uint8_t register address
/*
/*	@param[in] uint8_t value
/************************************************************************/
void rf24_write_register(unsigned short reg, unsigned short value);

/************************************************************************/
/* @brief	Function for pushing 1 byte through SPI
/*
/* @param[in] uint8_t 8 bit value to push
/************************************************************************/
void spi_transfer(uint8_t value);

/************************************************************************/
/* @brief	Function for getting 8bit value from RF24 register
/*
/* @param[in] uint8_t address to read
/* @param[in] uint8_t number of bytes to receive
/*
/* @return	  uint8_t array
/************************************************************************/
void rf24_get_register(uint8_t reg, uint8_t number_to_receive, uint8_t * result);

/************************************************************************/
/* @brief	Setting an address with length defined in RF_ADDR_LEN as
/*			0x12 * RF_ADDR_LEN for both transmitter and receiver
/************************************************************************/
void rf24_set_addr(uint8_t role, uint8_t * tx_addr, uint8_t * rx_addr);

/************************************************************************/
/* @brief	Payload transmit function. 
/*
/* @param[in]	uint8_t buffer address
/* @param[in]	uint8_t number of bytes to transmit 
/************************************************************************/
void rf24_tx(uint8_t * buffer, uint8_t len);

/************************************************************************/
/* @brief Function for getting status only
/*
/* @return 8bit nrf24 status                    
/************************************************************************/
uint8_t rf24_get_status(void);

/************************************************************************/
/* @brief Function for read all registers with UART
/************************************************************************/
void rf24_debug();

/************************************************************************/
/* @brief	Function for reading RX payload
/*
/* @param[in]	array with size [number_to_receive]
/* @param[in]	number of bytes to receive
/*
/* @return	Array with received data
/************************************************************************/   
void rf24_rx(uint8_t * data, uint8_t number_to_receive);
/************************************************************************/
/* @brief	Function for flushing RX/TX fifo
/*
/* @param[in]	RX/TX address
/************************************************************************/
void rf24_flush(uint8_t rxtx);
/************************************************************************/
/* @brief	Reset status register manually
/************************************************************************/
void rf24_reset();

#endif /* RADIORF24_H_ */