/*
 * twi.h
 *
 * Created: 20.08.2022 16:15:54
 *  Author: filek
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "config.h"

#define TWI_TIMEOUT 1600

#define TWI_START 0x08
#define TWI_RSTART 0x10
#define TWI_ADDR_ACK 0x18
#define TWI_ADDR_NACK 0x20
#define TWI_DATA_ACK 0x28
#define TWI_DATA_NACK 0x30

#define TWIR_ADDR_ACK 0x40
#define TWIR_ADDR_NACK 0x48
#define TWIR_DATA_ACK 0x50
#define TWIR_DATA_NACK 0x58

#define TWI_ERROR 0x38
#define TWI_NONE 0xF8

enum 
{
	TWI_OK,
	TWI_ERROR_START,
	TWI_ERROR_RSTART,
	TWI_NACK
};

void TwiInit(uint32_t speed);
uint8_t TwiRead(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);
uint8_t TwiWrite(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length);

#endif /* TWI_H_ */