/*
 * Transmitter.c
 *
 * Created: 09.08.2022 16:30:30
 * Author : filek
 */ 


#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "avr/interrupt.h"
#include "twi.h"
#include "spi.h"
#include "nrf24.h"
#include "motor.h"

const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t role = 1;

int main(void)
{	
	/*
	DDRC |= (1<<DDC0) | (1<<DDC1);
	
	Nrf24Radio radio;
	Nrf24Init(&radio);
	
	UartInit(9600, 0);
	SpiMasterInit();
	sei();
	
	if(Nrf24Begin(&radio) == 0)
	{
		UartStringSend("Could not start radio!");
		return 0;
	}
	UartStringSend("Radio started!");
	
	if(role)
	{
		Nrf24OpenWritingPipe(&radio, pipe);
	}
	else
	{
		Nrf24OpenReadingPipe(&radio, pipe);	
	}
	
	if(role == 0)
	{
		Nrf24StartListening(&radio);	
	}
	
	char msg[] = "abcdefghijk";
	*/
	
	MotorInit();
	sei();

    while (1) 
    {
		
		
		/*
		if(role == 1)
		{
			uint8_t status = Nrf24Write(&radio, (uint8_t*)msg, sizeof(msg));
			if(status)
			{
				PORTC |= (1 << DDC0);
			}
			else
			{
				UartByteSend('0');
			}
			_delay_ms(500);
			PORTC &= ~(1 << DDC0);
			_delay_ms(500);
			
			for(uint8_t i = 0; i < sizeof(msg) -1; i++)
			{
				msg[i] += 1;
				if(msg[i] == 'l') msg[i]='a';
			}
		}
		else
		{
			_delay_ms(200);
			if(Nrf24Available(&radio))
			{
				char rmsg[sizeof(msg)];
				Nrf24Read(&radio, (uint8_t*)rmsg, sizeof(rmsg));
				UartStringSend(rmsg);
			}
		}
		*/
    }
}

