/*
 * uart.h
 *
 * Created: 2022-12-28 23:55:38
 *  Author: Alek
 */ 


#ifndef UART_H_
#define UART_H_

#include "config.h"

#define RXBUFFERSIZE 128

typedef void (*UartCallback)(uint8_t*, uint16_t);

void UartInit(uint32_t baud);
void UartCallbackSet(UartCallback CallbackFunc, uint16_t DesiredLength);
void UartByteSend(uint8_t byte);
void UartArraySend(uint8_t* byteArray, uint16_t length);
void UartStringSend(uint8_t* str);
uint16_t UartCountRead();
uint8_t UartRead();



#endif /* UART_H_ */