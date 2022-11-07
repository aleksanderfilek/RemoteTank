/*
 * uart.c
 *
 * Created: 17.08.2022 22:23:49
 *  Author: filek
 */ 

#include "uart.h"
#include <avr/interrupt.h>

volatile static uint8_t rxBuffer[RXBUFFERSIZE] = {0};
volatile static uint16_t rxCount = 0;
volatile static uint8_t uartTxBusy = 1;

ISR(USART_RX_vect)
{
	volatile static uint16_t rxWritePos = 0;
	
	rxBuffer[rxWritePos] = UDR0;
	rxCount++;
	rxWritePos++;
	if(rxWritePos >= RXBUFFERSIZE)
	{
		rxWritePos = 0;
	}
}

ISR(USART_TX_vect)
{
	uartTxBusy = 1;
}

void UartInit(uint32_t baud, uint8_t highSpeed)
{
	uint8_t speed = 16;
	
	if(highSpeed != 0)
	{
		speed = 8;
		UCSR0A |= 1 << U2X0;
	}
	
	baud = (F_CPU/(speed*baud)) - 1;
	
	UBRR0H = (baud & 0xF00) >> 8;
	UBRR0L = (baud & 0x00FF);
	
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0) | (1 << TXCIE0);	
}

void UartByteSend(uint8_t byte)
{
	while(uartTxBusy == 0);
	uartTxBusy = 0;
	UDR0 = byte;
}

void UartArraySend(uint8_t* byteArray, uint16_t length)
{
	for(uint16_t i = 0; i < length; i++)
	{
		UartByteSend(byteArray[i]);
	}
}

void UartStringSend(uint8_t* str)
{
	uint16_t i = 0;
	
	do 
	{
		UartByteSend(str[i]);
		i++;
	} while (str[i] != '\0');
	UartByteSend(str[i]);
}

uint16_t UartCountRead()
{
	return rxCount;
}

uint8_t UartRead()
{
	static uint16_t rxReadPos = 0;
	uint8_t data = 0;
	
	data = rxBuffer[rxReadPos];
	rxReadPos++;
	rxCount--;
	if(rxReadPos >= RXBUFFERSIZE)
	{
		rxReadPos = 0;
	}
	
	return data;
}