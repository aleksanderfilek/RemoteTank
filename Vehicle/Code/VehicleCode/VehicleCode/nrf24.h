/*
 * nrf24.h
 *
 * Created: 2022-12-29 00:01:23
 *  Author: Alek
 */ 


#ifndef NRF24_H_
#define NRF24_H_


#include "config.h"

typedef struct Nrf24Radio
{
	uint8_t cePin;
	uint8_t csnPin;
	uint32_t speed;
	uint8_t payloadSize;
	uint8_t isPVariant;
	uint8_t isP0Rx;
	uint8_t addrWidth;
	uint8_t dynamicPayloadsEnabled;
	uint8_t csDelay;
	uint8_t pipe0ReadingAddress[5];
	uint8_t ackPayloadsEnabled;
	uint8_t configReg;
} Nrf24Radio;

void Nrf24Init(Nrf24Radio* Radio);
uint8_t Nrf24Begin(Nrf24Radio* Radio);
void Nrf24OpenWritingPipe(Nrf24Radio* Radio, uint64_t Pipe);
void Nrf24OpenReadingPipe(Nrf24Radio* Radio, uint64_t Pipe);
void Nrf24StartListening(Nrf24Radio* Radio);
void Nrf24StopListening(Nrf24Radio* Radio);
uint8_t Nrf24Write(Nrf24Radio* Radio, const uint8_t* data, uint8_t length);
void Nrf24Read(Nrf24Radio* Radio, uint8_t* data, uint8_t length);
uint8_t Nrf24Available(Nrf24Radio* Radio);
uint8_t Nrf24RxFifoEmpty(Nrf24Radio* Radio);

#endif /* NRF24_H_ */