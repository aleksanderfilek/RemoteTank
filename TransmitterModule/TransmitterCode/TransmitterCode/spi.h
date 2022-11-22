/*
 * spi.h
 *
 * Created: 17.08.2022 22:24:26
 *  Author: filek
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "config.h"

void SpiMasterInit(void);
//void SpiTransfer(uint8_t* tx, uint8_t* rx, uint16_t length);
uint8_t SpiSend(uint8_t data);
void SpiCSSet(uint8_t pin, uint8_t mode);

#endif /* SPI_H_ */