/*
 * spi.c
 *
 * Created: 17.08.2022 22:24:03
 *  Author: filek
 */ 

#include "spi.h"
#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile uint8_t spiRxData = 0;
volatile uint8_t spiTxRxDone = 0;

ISR(SPI_STC_vect)
{
	if(SPSR & (1<<SPIF))
	{
		spiRxData = SPDR;
		spiRxData = 0;
		spiTxRxDone = 1;
	}
	else
	{
		spiRxData = SPDR;
		spiTxRxDone = 1;
	}
}

void SpiMasterInit(void)
{
	DDRB |= (1 << DDB5) | (1 << DDB3);
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1<<SPIE);
}

uint8_t SpiSend(uint8_t data)
{
	spiTxRxDone = 0;
	SPDR = data;
	
	while(spiTxRxDone == 0);
	
	return spiRxData;
}

void SpiTransfer(uint8_t* tx, uint8_t* rx, uint16_t length)
{
	SpiCSSet(PORTB2, 1);
	
	for(uint16_t i = 0; i < length; i++)
	{
		rx[i] = SpiSend(tx[i]);
	}
	
	SpiCSSet(PORTB2, 0);
}

void SpiCSSet(uint8_t pin, uint8_t mode)
{
	if(mode == 1)
	{
		PORTB &= ~(1 << pin);
	}
	else
	{
		PORTB |= (1 << pin);
	}
}