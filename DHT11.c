#include "DHT11.h"


void dht11_read(uint8_t * data)
{
	unsigned int test_l[40] ;
	unsigned int test_h[40] ;
	uint8_t checksum = 0;
	for (int i = 0; i !=40; i++)
	{
		test_h[i] = 0;
		test_l[i] = 0;
	}
	/** Start read signal **/
	sbi(DHT11_DDR,DHT11_NUM);		
	cbi(DHT11_PORT,DHT11_NUM);
	_delay_ms(18);
	cbi(DHT11_DDR,DHT11_NUM);
	_delay_us(40);
	
	/** Read output **/
	for (uint8_t i = 0; i != 40; i++)
	{
		
		//** Reading data procedure **/
		while(!( DHT11_PIN & 0x01))
		{
			test_l[i] += 1;		//16 cycles of clock = 1us
		}
		while( (DHT11_PIN & 0x01) )
		{
			test_h[i] += 1;		//16 cycles of clock = 1us
		}
		
		//** Encode Humidity integral **//
		if(i <= 8 && i > 0){
			if (test_h[i] < 40)
			{
				data[0] <<= 1;
			}
			else
			{
				data[0] <<= 1;
				data[0] |= 1;
			}
		}
		//** Encode Humidity decimal **//
		if (i > 8 && i <= 16)
		{
			if (test_h[i] < 40)
			{
				data[1] <<= 1;
			}
			else
			{
				data[1] <<= 1;
				data[1] |= 1;
			}
		}
		//** Encode Temperature integral **//
		if (i >16 && i <= 24)
		{
			if (test_h[i] < 40)
			{
				data[2] <<= 1;
			}
			else
			{
				data[2] <<= 1;
				data[2] |= 1;
			}
		}
		//** Encode Temperature decimal **//
		if (i > 24 && i <= 32)
		{
			if (test_h[i] < 40)
			{
				data[3] <<= 1;
			}
			else
			{
				data[3] <<= 1;
				data[3] |= 1;
			}
		}
		//** Encode checksum **//
		if (i > 32 && i <= 40)
		{
			if (test_h[i] < 40)
			{
				checksum <<= 1;
			}
			else
			{
				checksum <<= 1;
				checksum |= 1;
			}
		}
		
	}
	
	//** Verify checksum **//
	//if (checksum < 31)
	//{
	//	data[0] = -127;
	//	data[1] = -127;
	//	data[2] = -127;
	//	data[3] = -127;
	//}
	_delay_ms(100);
	
	sbi(DHT11_DDR,DHT11_NUM);
	sbi(DHT11_PORT,DHT11_NUM);
}