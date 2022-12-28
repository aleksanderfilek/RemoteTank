/*
 * twi.c
 *
 * Created: 2022-12-28 23:59:22
 *  Author: Alek
 */ 


#include "twi.h"
#include <avr/interrupt.h>

volatile uint8_t status = TWI_NONE;

ISR(TWI_vect)
{
	status = (TWSR & TWI_NONE);
}

static uint8_t TwiStart(void)
{
	uint16_t i = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 <<TWIE);
	
	while(status != TWI_START)
	{
		i++;
		if(i >= TWI_TIMEOUT)
		{
			return TWI_ERROR_START;
		}
	}
	
	return TWI_OK;
}

static void TwiStop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN) | (1 <<TWIE);
}

static uint8_t TwiRestart(void)
{
	uint16_t i = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 <<TWIE);
	
	while(status != TWI_RSTART)
	{
		i++;
		if(i >= TWI_TIMEOUT)
		{
			return TWI_ERROR_START;
		}
	}
	
	return TWI_OK;
}

static uint8_t TwiAddrWriteAck(void)
{
	uint16_t i = 0;
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWIE);
	while(status != TWI_ADDR_ACK)
	{
		i++;
		if(i >= TWI_TIMEOUT)
		{
			return TWI_ERROR_START;
		}
	}
	
	return TWI_OK;
}

static uint8_t TwiDataWriteAck(void)
{
	uint16_t i = 0;
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWIE);
	while(status != TWI_DATA_ACK)
	{
		i++;
		if(i >= TWI_TIMEOUT)
		{
			return TWI_ERROR_START;
		}
	}
	
	return TWI_OK;
}

static uint8_t TwiAddrReadAck(void)
{
	uint16_t i = 0;
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWIE);
	while(status != TWIR_ADDR_ACK)
	{
		i++;
		if(i >= TWI_TIMEOUT)
		{
			return TWI_ERROR_START;
		}
	}
	
	return TWI_OK;
}
static uint8_t TwiDataReadAck(uint8_t ack)
{
	uint16_t i = 0;
	if(ack != 0)
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWIE) | (1 << TWEA);
		while(status != TWIR_DATA_ACK)
		{
			i++;
			if(i >= TWI_TIMEOUT)
			{
				return TWI_ERROR_START;
			}
		}
	}
	else
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWIE);
		while(status != TWIR_DATA_NACK)
		{
			i++;
			if(i >= TWI_TIMEOUT)
			{
				return TWI_ERROR_START;
			}
		}
	}
	
	return TWI_OK;
}

void TwiInit(uint32_t speed)
{
	uint32_t gen_t = 0;
	gen_t = (((F_CPU/speed) - 16)/2) & 0xFF;
	TWBR = gen_t & 0xFF;
	TWCR = (1 << TWEN) | (1 << TWIE);
}

uint8_t TwiRead(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length)
{
	uint16_t i = 0;
	uint8_t err = TWI_OK;
	
	err = TwiStart();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	TWDR = (addr << 1) | 0;
	
	err = TwiAddrWriteAck();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	TWDR = reg;
	err = TwiDataWriteAck();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	err = TwiRestart();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	TWDR = (addr << 1) | 1;
	
	err = TwiAddrReadAck();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	for(i = 0; i < (length - 1); i++)
	{
		err = TwiDataReadAck(1);
		if(err != TWI_OK)
		{
			TwiStop();
			return err;
		}
		data[i] = TWDR;
	}
	
	err = TwiDataReadAck(0);
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	data[i] = TWDR;
	
	TwiStop();
	
	return err;
}

uint8_t TwiWrite(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length)
{
	uint16_t i = 0;
	uint8_t err = TWI_OK;
	
	err = TwiStart();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	TWDR = (addr << 1) | 0;
	
	err = TwiAddrWriteAck();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	TWDR = reg;
	err = TwiDataWriteAck();
	if(err != TWI_OK)
	{
		TwiStop();
		return err;
	}
	
	for(i = 0; i < length; i++)
	{
		TWDR = data[i];
		err = TwiDataWriteAck();
		if(err != TWI_OK)
		{
			TwiStop();
			return err;
		}
	}
	
	TwiStop();
	
	return err;
	
}