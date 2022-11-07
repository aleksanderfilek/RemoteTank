/*
 * uart.h
 *
 * Created: 17.08.2022 22:23:38
 *  Author: filek
 */ 


#ifndef UART_H_
#define UART_H_

#include "config.h"

#define RXBUFFERSIZE 128

void UartInit(uint32_t baud, uint8_t highSpeed);
void UartByteSend(uint8_t byte);
void UartArraySend(uint8_t* byteArray, uint16_t length);
void UartStringSend(uint8_t* str);
uint16_t UartCountRead();
uint8_t UartRead();

#endif /* UART_H_ */