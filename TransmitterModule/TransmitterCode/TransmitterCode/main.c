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

int main(void)
{
	uint8_t data = 'A';
	
	UartInit(9600, 0);
	
	sei();

    while (1) 
    {
		UartByteSend(data);
		data++;
		if(data == 'Z')
		{
			data = 'A';
		}
		
		_delay_ms(500);
    }
}

