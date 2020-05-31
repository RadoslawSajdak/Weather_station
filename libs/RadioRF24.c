/*
 * Weather station sensor - Radio module
 * Created: 2020-04-17 20:21:02
 * Author : Sajdak Radoslaw
 * Description: Custom library for nRF24L01 radio
 * Features: Radio communication, SPI
 */ 
#include "RadioRF24.h"

/** Addresses for devices. Should be different **/
uint8_t tx_addr[RF_ADDR_LEN] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_addr[RF_ADDR_LEN] = {0xE7,0xE7,0xE7,0xE7,0xE7};

void rf24_init(uint8_t role,uint8_t channel, uint8_t payload)
{
	uint8_t temp;
	DDRB |= (1<<SCK) | (0<<MISO) | (1<<MOSI) | (1<<CSN) | (1<<CE);
	sbi(RF_PORT,CSN);

	if (role == 0) temp = 0x5a;
	else temp = 0x3b;			
	cbi(RF_PORT,CE);
	rf24_write_register(CHANNEL,channel);
	rf24_write_register(RX_PW_P0, 0x00);	
	rf24_write_register(RX_PW_P1, payload);	
	rf24_write_register(RF_SETUP,0x06);		
	rf24_write_register(CONFIG,RF24_CONFIG);
	rf24_write_register(EN_AA,0x03);
	rf24_write_register(EN_RX,0x03);		
	rf24_write_register(SETUP_RETR,0x4F);
	rf24_set_addr(role,tx_addr,rx_addr);
	rf24_flush(FLUSH_RX);
	rf24_write_register(STATUS,0x70);
	rf24_write_register(CONFIG, temp);
	
	_delay_ms(100);
	sbi(RF_PORT,CE);
	_delay_ms(120);
	

}
void rf24_set_addr(uint8_t role, uint8_t * tx_addr, uint8_t * rx_addr)
{
	
	/** Setting TX address of device **/
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	spi_transfer(W_REGISTER | (TX_ADDR & REG_MASK ));
	for(int i = 0; i < RF_ADDR_LEN; i++)
	{
		if(role != 0)
		{
			spi_transfer(tx_addr[i]);
		}
		else
			spi_transfer(rx_addr[i]);
	}
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);

	/** Setting RX pipe 0 address of device **/
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	spi_transfer(W_REGISTER | (RX0_ADDR & REG_MASK ));
	for(int i = 0; i < RF_ADDR_LEN; i++)
	{
		if(role != 0)
		{
			spi_transfer(tx_addr[i]);
		}
		else
			spi_transfer(rx_addr[i]);
	}
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);
	
	/** Setting RX pipe 1 address of device **/
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	spi_transfer(W_REGISTER | (RX1_ADDR & REG_MASK ));
	for(int i = 0; i < RF_ADDR_LEN; i++)
	{
		if(role == 0)
		{
			spi_transfer(tx_addr[i]);
		}
		else
			spi_transfer(rx_addr[i]);
	}
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);

	
}

void spi_transfer(uint8_t value)
{
	for (uint8_t i = 0; i<8; i++)
	{
		if (value & 0x80)
			sbi(RF_PORT, MOSI);
		else
			cbi(RF_PORT, MOSI);
		sbi(RF_PORT, SCK);
		value <<= 1;
		_delay_us(0.2);
		cbi(RF_PORT, SCK);
	}
}

void rf24_get_register(uint8_t reg, uint8_t number_to_receive, uint8_t * result)
{
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	
	/** Write address with R_REGISTER prefix **/
	reg = (R_REGISTER | (reg & REG_MASK ));
	spi_transfer(reg);
	
	/** Reading data from SPI **/
	_delay_us(12);
	for (uint8_t n = 0; n < number_to_receive; n++ )
	{
		for (uint8_t i = 0; i<8; i++)
		{
			result[n] <<= 1;
			if (PINB & (1 << MISO))
			{
				result[n] |= 1;
			}
			sbi(RF_PORT, SCK);
			_delay_us(0.2);
			cbi(RF_PORT, SCK);
		}
	
	}
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);
}

void rf24_write_register(unsigned short reg, unsigned short value)
{
	//Write address	with W_REGISTER prefix
	reg = (W_REGISTER | (reg & REG_MASK ));
	cbi(RF_PORT,CSN);
	_delay_us(12);

	spi_transfer(reg);
	spi_transfer(value);
	sbi(RF_PORT, CSN);
	
}

void rf24_tx(uint8_t * bufer, uint8_t len)
{
	/** Make sure memory is clear **/
	rf24_flush(FLUSH_TX);
	rf24_flush(FLUSH_RX);

	_delay_ms(12);
	cbi(RF_PORT,CE);

	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);

	/** Write TX fifo **/
	spi_transfer(TX_PAYLOAD);
	_delay_us(12);
	for (int i = 0 ; i < len; i++)
	{
		spi_transfer(bufer[i]);
		_delay_us(12);
	}
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);

	/** Send impulse **/
	_delay_ms(12);
	sbi(RF_PORT,CE);
	_delay_us(20);
	cbi(RF_PORT,CE);

	rf24_reset();

}

void rf24_rx(uint8_t * data, uint8_t number_to_receive, uint8_t * lcd_state)
{
	
	_delay_ms(10);
	sbi(RF_PORT,CE);
	_delay_us(50);

	/** Wait for data in fifo **/
	while( !(rf24_get_status() & (1 << RX_DR)) & (lcd_state[0] == 3));
	_delay_ms(10);
	cbi(RF_PORT,CE);
	_delay_us(50);
	cbi(RF_PORT,CSN);
	_delay_us(10);
	
	/** Read data from fifo **/
	spi_transfer(RX_PAYLOAD);
	_delay_us(12);
	for (uint8_t n = 0; n < number_to_receive; n++ )
	{
		for (uint8_t i = 0; i<8; i++)
		{
			data[n] <<= 1;
			if (PINB & (1 << MISO))
			{
				data[n] |= 1;
			}
			sbi(RF_PORT, SCK);
			_delay_us(0.2);
			cbi(RF_PORT, SCK);
		}
		
	}
	_delay_us(10);
	sbi(RF_PORT,CSN);
	_delay_us(10);
	
	_delay_us(10);
	rf24_write_register(STATUS,(1 << RX_DR));

	
}

void rf24_flush(uint8_t rxtx)
{
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	
	spi_transfer(rxtx);
	
	_delay_us(12);
	sbi(RF_PORT, CSN);
	_delay_us(12);
}

uint8_t rf24_get_status(void)
{
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	
	/** Read status from SPI. It is always coming after falling edge
	on CSN pin **/
	uint8_t value = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		value <<= 1;
		if (PINB & (1 << MISO))
		{
			value |= 1;
		}
		sbi(RF_PORT, SCK);
		_delay_us(0.2);
		cbi(RF_PORT, SCK);
	}
	
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);
	
	return value;
}

void rf24_reset()
{
	_delay_us(12);
	cbi(RF_PORT,CSN);
	_delay_us(12);
	
	/** Write default value to reset interrupts **/
	spi_transfer(W_REGISTER | (STATUS & REG_MASK));
	_delay_us(12);
	spi_transfer(0x70);
	_delay_us(12);
	
	_delay_us(12);
	sbi(RF_PORT,CSN);
	_delay_us(12);
}
/*void rf24_debug()
{
	uint8_t debug_code[1] ;
	
	rf24_get_register(CONFIG,1,debug_code);
	printf("CONFIG: %x\n\r", debug_code[0]);
	rf24_get_register(EN_AA,1,debug_code);
	printf("EN_AA: %x\n\r", debug_code[0]);
	rf24_get_register(EN_RX,1,debug_code);
	printf("EN_RX: %x\n\r", debug_code[0]);
	rf24_get_register(SETUP_AW,1,debug_code);
	printf("SETUP_AW: %x\n\r", debug_code[0]);
	rf24_get_register(SETUP_RETR,1,debug_code);
	printf("SETUP_RETR: %x\n\r", debug_code[0]);
	rf24_get_register(CHANNEL,1,debug_code);
	printf("CHANNEL: %x\n\r", debug_code[0]);
	rf24_get_register(RF_SETUP,1,debug_code);
	printf("RF_SETUP: %x\n\r", debug_code[0]);
	rf24_get_register(STATUS,1,debug_code);
	printf("STATUS: %x\n\r", debug_code[0]);
	rf24_get_register(FIFO_STATUS,1,debug_code);
	printf("FIFO_STATUS: %x\n\r", debug_code[0]);
}
*/