/*
 * nrf24.c
 *
 * Created: 07.11.2022 14:11:12
 *  Author: filek
 */ 
#include "nrf24.h"
#include "spi.h"
#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define min(a,b) (a < b)? a : b;
#define STARTUP_DELAY                 150             /*in milliseconds*/
#define POWER_DOWN_DELAY              20
#define STANDBYI_DELAY                2
#define PRX_MODE_DELAY                100
#define ADDRESS_WIDTH_DEFAULT         5               /*address width in bytes, for default value*/
#define RF_CHANNEL_DEFAULT            32
#define RF_DATARATE_DEFAULT           1000            /*250, 1000, 2000*/
#define RF_PWR_DEFAULT                6               /*0, -6, -12, -18*/
#define STATIC_PAYLOAD_WIDTH_DEFAULT  1               /*for static payload mode, configurable between 1 and 32 bytes for PRX device ONLY (RX_PW_Pn, n for data pipe n)(no register for payload length in PTX device)*/
#define NUMBER_OF_DP_DEFAULT          1               /*number of datapipes, 1 to 6*/
#define RETRANSMIT_DELAY_DEFAULT      500
#define RETRANSMIT_COUNT_DEFAULT      2

#define OPEN                          1
#define CLOSE                         0
#define ENABLE                        1
#define DISABLE                       0
#define HIGH	                      1
#define LOW							  0

#define CONFIG_REGISTER_DEFAULT       0X08
#define EN_AA_REGISTER_DEFAULT        0X3F
#define EN_RXADDR_REGISTER_DEFAULT    0X00
#define SETUP_AW_REGISTER_DEFAULT     0X03
#define SETUP_RETR_REGISTER_DEFAULT   0X03
#define RF_CH_REGISTER_DEFAULT        0X02
#define RF_SETUP_REGISTER_DEFAULT     0X0E
#define STATUS_REGISTER_DEFAULT       0X0E
#define MAXIMUM_NUMBER_OF_DATAPIPES   6

#define POWER_DOWN                    0X00
#define STANDBYI                      0X01
#define STANDBYII                     0X02
#define PTX                           0X03
#define PRX                           0X04
#define DEVICE_NOT_INITIALIZED        0X05

#define POWER_SAVING                  0X02
#define TURN_OFF                      0X03

#define RESET                         1
#define NO_RESET                      0
#define NO_ACK_MODE                   1
#define ACK_MODE                      0
#define TRANSMIT_BEGIN                1
#define TRANSMIT_FAIL                 0
#define TRANSMIT_IN_PROGRESS          0
#define TRANSMIT_DONE                 1
#define TRANSMIT_FAILED               0XFF
#define OPERATION_DONE                1
#define OPERATION_ERROR               0
#define RECEIVE_FIFO_EMPTY            2
#define TX_BUFFER                     1
#define RX_BUFFER                     0

/*bits definition section*/
#define MASK_RX_DR          6               /*mask interrupt caused by RX_DR: 1 interrupt not reflected on IRQ pin (IRQ is active low), inside CONFIG register*/
#define MASK_TX_DS          5               /*mask interrupt caused by TX_DS: 1 interrupt not reflected on IRQ pin (IRQ is active low), inside CONFIG register*/
#define MASK_MAX_RT         4               /*mask interrupt caused by MAX_RT means maximum number of retransmissions reached: 1 interrupt not reflected on IRQ pin (IRQ is active low), inside CONFIG register*/
#define EN_CRC              3               /*enale CRC, forced high if one of the bits in EN_AA is high, inside CONFIG register*/
#define CRCO                2               /*CRC encoding scheme, 0 is 1 byte, 1 is 2 bytes, inside CONFIG register*/
#define PWR_UP              1               /*1 is power up, inside CONFIG register*/
#define PRIM_RX             0               /*RX/TX control, 1: PRX, inside CONFIG register*/
#define ENAA_P5             5               /*enable auto acknowledgement data pipe 5*/
#define ENAA_P4             4
#define ENAA_P3             3
#define ENAA_P2             2
#define ENAA_P1             1
#define ENAA_P0             0
#define ERX_P5              5               /*part of EN_RXADDR, enable data pipe 5*/
#define ERX_P4              4
#define ERX_P3              3
#define ERX_P2              2
#define ERX_P1              1
#define ERX_P0              0
#define AW_1                1               /*RX/TX address field width, 00 illegal, 01 3 bytes, 10 4 bytes, 11 5 bytes*/
#define AW_0                0
#define ARD_3               7               /*auto retransmit delay, 0000 250us, 0001 500us ...> 1111 4000us*/
#define ARD_2               6
#define ARD_1               5
#define ARD_0               4
#define ARC_3               3               /*auto retransmit count, 0000 retransmit deisabled, 1111 up to 15 retransmit on failure of AA. (inside SETUP_RETR register)*/
#define ARC_2               2
#define ARC_1               1
#define ARC_0               0
#define RF_CH_6             6               /*sets the frequencvy channel nRF24L01+ operates on*/
#define RF_CH_5             5
#define RF_CH_4             4
#define RF_CH_3             3
#define RF_CH_2             2
#define RF_CH_1             1
#define RF_CH_0             0
#define CONT_WAVE           7               /*enables continuous carrier transmit when high*/
#define RF_DR_LOW           5               /*sets the RF data rate to 250kbps*/
#define PLL_LOCK            4               /*force PLL lock signal. used for testing ONLY*/
#define RF_DR_HIGH          3               /*select between high speed data rates and works ONLY when RF_DR_LOW is 0. 0 for 1Mbps, 1 for 2Mbps*/
#define RF_PWR_1            2
#define RF_PWR_0            1
#define RX_DR               6               /*IRQ for new packet in RX FIFO (newly received)*/
#define TX_DS               5               /*IRQ for ACK received in TX mode*/
#define MAX_RT              4
#define RX_P_NO_2           3
#define RX_P_NO_1           2
#define RX_P_NO_0           1
#define TX_FULL             0
#define PLOS_CNT_3          7               /*inside OBSERVE_TX register, counts the total number of retransmissions since last channel change. reset by writing to RF_CH*/
#define PLOS_CNT_2          6
#define PLOS_CNT_1          5
#define PLOS_CNT_0          4
#define ARC_CNT_3           3               /*inside OBSERVE_TX register, counts the number of retransmissions for current transaction. reset by initiating new transaction*/
#define ARC_CNT_2           2
#define ARC_CNT_1           1
#define ARC_CNT_0           0
#define RPD                 0               /*received power detector, if received power is less than -64dbm, RPD = 0*/
#define TX_REUSE            6
#define FIFO_TX_FULL        5
#define TX_EMPTY            4
#define RX_FULL             1
#define RX_EMPTY            0
#define DPL_P5              5
#define DPL_P4              4
#define DPL_P3              3
#define DPL_P2              2
#define DPL_P1              1
#define DPL_P0              0                 /*must be set on PTX in dynamic payload length mode*/
#define EN_DPL              2                 /*set to enable dynamic payload length*/
#define EN_ACK_PAY          1                 /*used to enable auto acknowledgement with payload in PRX (inside FEATURE register)*/
#define EN_DYN_ACK          0                 /**/

/*registers definition section*/
#define CONFIG_ADDRESS              0X00
#define EN_AA_ADDRESS               0X01              /*enable auto acknowledgement feature*/
#define EN_RXADDR_ADDRESS           0X02              /*register containing bits to enable 6 data pipes individually*/
#define SETUP_AW_ADDRESS            0X03              /*address field length is configured in here to be 3, 4 or 5 bytes long*/
#define SETUP_RETR_ADDRESS          0X04              /*setup ARC bits to configure auto retransmission count*/
#define RF_CH_ADDRESS               0X05
#define RF_SETUP_ADDRESS            0X06
#define STATUS_ADDRESS              0X07              /*contains RX_DR, TX_DS, MAX_RT, RX_P_NO, TX_FULL, send R_REGISTER then NOP to read*/
#define OBSERVE_TX_ADDRESS          0X08              /*contains ARC_CNT and PLOS_CNT, two counters for retransmission. these counters could be used to assess the network quality*/
#define RPD_REG_ADDRESS             0X09
#define RX_ADDR_P0_ADDRESS          0X0A              /*the address for PRX device. if a packet contains this address, enhanced shockburst starts validating the packet*/
#define RX_ADDR_P1_ADDRESS          0X0B              /*a total of 6 unique addresses could be assigned to a PRX device (Multiceiver feature)*/
#define RX_ADDR_P2_ADDRESS          0X0C              /*these addresses must NOT be the same*/
#define RX_ADDR_P3_ADDRESS          0X0D
#define RX_ADDR_P4_ADDRESS          0X0E
#define RX_ADDR_P5_ADDRESS          0X0F
#define TX_ADDR_ADDRESS             0X10              /*40 bits long register, transmit address, used for a PTX device only. configure address legth in SETUP_AW register. set RX_ADDR_P0 equal to this address to handle automatic acknowledge*/
#define RX_PW_P0_ADDRESS            0X11              /*these registers are for setting the static payload length in static payload length mode (receiver side)*/
#define RX_PW_P1_ADDRESS            0X12
#define RX_PW_P2_ADDRESS            0X13
#define RX_PW_P3_ADDRESS            0X14
#define RX_PW_P4_ADDRESS            0X15
#define RX_PW_P5_ADDRESS            0X16
#define FIFO_STATUS_ADDRESS         0X17
#define DYNPD_ADDRESS               0X1C              /*on receiver side (RX mode), this register must be set to enable dynamic payload length. a PTX in dynamic mode, must have the DYNPD_P0 set*/
#define FEATURE_ADDRESS             0X1D              /*contains the EN_DPL bit to enable dynamic payload length*/

/*commands definition section*/
#define R_REGISTER          0X00              /*read commmand and STATUS registers, 5 bit register map address*/
#define W_REGISTER          0X20              /*write commmand and STATUS registers, 5 bit register map address, executable in POWER DOWN or STANDBY modes only*/
#define R_RX_PAYLOAD        0X61              /*read RX payload, 1-32 bytes. read operation starts at byte 0. payload is deleted from FIFO after its read*/
#define W_TX_PAYLOAD        0XA0              /*write TX payload, starts at byte 0, 1-32 bytes*/
#define FLUSH_TX            0XE1              /*flush TX FIFO, used in TX mode*/
#define FLUSH_RX            0XE2              /*flush RX FIFO, used in RX mode*/
#define REUSE_TX_PL         0XE3              /*used for a PTX device, reuse last transmitted payload for an exact number. alternative to auto retransmission*/
#define R_RX_PL_WID         0X60              /*command for receiver side, in order to read the payload length in dynamic payload length mode*/
#define W_ACK_PAYLOAD       0XA0              /*used in RX mode, to write payload in TX FIFO and later transmit the payloads along with ACK packet to PTX, if DPL is enabled*/
#define W_TX_PAYLOAD_NOACK  0XB0              /*used in TX mode, disables AUTOACK on this specific packet. must be first enabled in FEATURE register by setting the EN_DYN_ACK bit. if used, PTX will not wait for ACK and goes directly to standby I*/
#define NOP_CMD             0XFF              /*might be used to read the status register*/
#define ACTIVATE			0x50

static void PinSet(uint8_t pin, uint8_t state)
{
	if(state == LOW)
	{
		PORTB &= ~(1 << pin);
	}
	else
	{
		PORTB |= (1 << pin);
	}
}

static void Write(Nrf24Radio* Radio, uint8_t address, uint8_t *value, uint8_t length)
{
	PinSet(Radio->csnPin, LOW);
	SpiSend(W_REGISTER | address);
	for(uint8_t i = 0; i < length; i++)
	{
		SpiSend(value[i]);
	}
	PinSet(Radio->csnPin, HIGH);
}

void Read(Nrf24Radio* Radio, uint8_t address, uint8_t* value, uint8_t length)
{
	PinSet(Radio->csnPin, LOW);
	SpiSend(R_REGISTER | address);
	for(uint8_t i = 0; i < length; i++)
	{
		value[i] = SpiSend(NOP_CMD);
	}
	PinSet(Radio->csnPin, HIGH);
}


static uint8_t WriteRegister(Nrf24Radio* Radio, uint8_t Addr, uint8_t value)
{
	PinSet(Radio->csnPin, LOW);
	
	uint8_t status = SpiSend(W_REGISTER | Addr);
	SpiSend(value);
	
	PinSet(Radio->csnPin, HIGH);
	
	return status;
}

static uint8_t WriteCommand(Nrf24Radio* Radio, uint8_t Addr)
{
	PinSet(Radio->csnPin, LOW);
	
	uint8_t status = SpiSend(W_REGISTER | Addr);
	
	PinSet(Radio->csnPin, HIGH);
	
	return status;
}

static uint8_t ReadRegister(Nrf24Radio* Radio, uint8_t Addr)
{
	PinSet(Radio->csnPin, LOW);
	
	SpiSend(R_REGISTER | Addr);
	uint8_t result = SpiSend(NOP_CMD);
	
	PinSet(Radio->csnPin, HIGH);
	
	return result;
}

static void ToggleFeatures(Nrf24Radio* Radio)
{
	PinSet(Radio->csnPin, LOW);
	SpiSend(ACTIVATE);
	SpiSend(0x73);
	PinSet(Radio->csnPin, HIGH);
}

static void PowerUp(Nrf24Radio* Radio)
{
	if (!(Radio->configReg & (1<<PWR_UP))) {
		Radio->configReg |= (1<<PWR_UP);
		WriteRegister(Radio, CONFIG_ADDRESS, Radio->configReg);
		_delay_us(5000);
	}
}

static void PowerDown(Nrf24Radio* Radio)
{
	PinSet(Radio->cePin, LOW);
	Radio->configReg &= ~(1<<PWR_UP);
	Write(Radio, CONFIG_ADDRESS, &Radio->configReg, 1);
}

static void TxFlush(Nrf24Radio* Radio)
{
	WriteCommand(Radio, FLUSH_TX);
}

static void RxFlush(Nrf24Radio* Radio)
{
	WriteCommand(Radio, FLUSH_RX);
}

void Nrf24Init(Nrf24Radio* Radio)
{
	Radio->cePin = DDB1;
	Radio->csnPin = DDB2;
	
	DDRB |= (1<<DDB1) | (1<<DDB2);
	
	Radio->speed = 10000000;
	Radio->payloadSize = 32;
	Radio->isPVariant = 0;
	Radio->isP0Rx = 0;
	Radio->addrWidth = 5;
	Radio->dynamicPayloadsEnabled = 1;
	Radio->csDelay = 5;
	Radio->pipe0ReadingAddress[0] = 0;
}

uint8_t Nrf24Begin(Nrf24Radio* Radio)
{
	PinSet(Radio->cePin, LOW);
	PinSet(Radio->csnPin, HIGH);
	
	_delay_ms(5);
	
	// setup retransmition
	WriteRegister(Radio, SETUP_RETR_ADDRESS, (5 << ARD_0) | 15);
	
	// setup rf
	uint8_t dataRate = ReadRegister(Radio, RF_SETUP_ADDRESS);
	dataRate = dataRate & ~((1 << RF_DR_LOW) | (1 << RF_DR_HIGH));
	WriteRegister(Radio, RF_SETUP_ADDRESS, dataRate);
	
	// toggle features command
	uint8_t beforeToggle = ReadRegister(Radio, FEATURE_ADDRESS);
	ToggleFeatures(Radio);
	uint8_t afterToggle = ReadRegister(Radio, FEATURE_ADDRESS);
	Radio->isPVariant = beforeToggle == afterToggle;
	if(afterToggle)
	{
		if(Radio->isPVariant)
		{
			ToggleFeatures(Radio);
		}
		
		WriteRegister(Radio, FEATURE_ADDRESS, 0);
	}
	
	Radio->ackPayloadsEnabled = 0;
	WriteRegister(Radio, DYNPD_ADDRESS, 0);
	
	Radio->dynamicPayloadsEnabled = 0;
	WriteRegister(Radio, EN_AA_ADDRESS, 0x3F);
	WriteRegister(Radio, EN_RXADDR_ADDRESS, 3);

	for (uint8_t i = 0; i < 6; ++i) {
		WriteRegister(Radio, RX_PW_P0_ADDRESS + i, Radio->payloadSize);
	}
	
	WriteRegister(Radio, SETUP_AW_ADDRESS, 3);

	WriteRegister(Radio, RF_CH_ADDRESS, 101);

	WriteRegister(Radio, STATUS_ADDRESS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT));

	TxFlush(Radio);
	RxFlush(Radio);
	
	WriteRegister(Radio, CONFIG_ADDRESS, (1<<EN_CRC) | (1<<CRCO));
	Radio->configReg = ReadRegister(Radio, CONFIG_ADDRESS);
	
	PowerUp(Radio);
	
	return Radio->configReg == ((1<<EN_CRC) | (1<<CRCO) | (1<<PWR_UP))?1:0;
}

void Nrf24OpenWritingPipe(Nrf24Radio* Radio, uint64_t Pipe)
{
	Write(Radio, RX_ADDR_P0_ADDRESS, (uint8_t*)&Pipe, Radio->addrWidth);
	Write(Radio, TX_ADDR_ADDRESS, (uint8_t*)&Pipe, Radio->addrWidth);
}

void Nrf24OpenReadingPipe(Nrf24Radio* Radio, uint64_t Pipe)
{
	Write(Radio, RX_ADDR_P1_ADDRESS, (uint8_t*)&Pipe, Radio->addrWidth);
	
	uint8_t enrx = ReadRegister(Radio, EN_RXADDR_ADDRESS);
	enrx |= (1<<ERX_P1);
	WriteRegister(Radio, EN_RXADDR_ADDRESS, enrx);
}

void Nrf24StartListening(Nrf24Radio* Radio)
{
	PowerUp(Radio);
	
	Radio->configReg |= (1<<PRIM_RX);
	WriteRegister(Radio, CONFIG_ADDRESS, Radio->configReg);
	
	WriteRegister(Radio, STATUS_ADDRESS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT));
	
	PinSet(Radio->cePin, HIGH);
	
	if(Radio->isP0Rx)
	{
		Write(Radio, RX_ADDR_P0_ADDRESS, Radio->pipe0ReadingAddress, Radio->addrWidth);
	}
	else
	{
		uint8_t enrx = ReadRegister(Radio, EN_RXADDR_ADDRESS);
		enrx &= ~(1<<ERX_P0);
		WriteRegister(Radio, EN_RXADDR_ADDRESS, enrx);
		Radio->isP0Rx = 0;
	}
}

static uint8_t GetStatus(Nrf24Radio* Radio)
{
	return WriteCommand(Radio, NOP_CMD);
}

uint8_t Nrf24Write(Nrf24Radio* Radio, const uint8_t* data, uint8_t length)
{
	uint8_t blankLen = !length ? 1 : 0;
	if(Radio->dynamicPayloadsEnabled == 0)
	{
		length = min(length, Radio->payloadSize);
		blankLen = Radio->payloadSize - length;
	}
	else
	{
		length = min(length, 32);
	}
	
	PinSet(Radio->csnPin, LOW);
	SpiSend(W_TX_PAYLOAD);
	while (length--) {
		SpiSend(*data++);
	}

	while (blankLen--) {
		SpiSend(0);
	}
	PinSet(Radio->csnPin, HIGH);
	
	PinSet(Radio->cePin, HIGH);
	PORTC |= (1 << DDC1);
	while(!(GetStatus(Radio) & ((1<<TX_DS) | (1<<MAX_RT)))){}
	PORTC &= ~(1 << DDC1);
	PinSet(Radio->cePin, LOW);
	
	uint8_t status = WriteRegister(Radio, STATUS_ADDRESS, (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT));

	if(status & (1<<MAX_RT))
	{
		TxFlush(Radio);
		return 0;
	}
	
	return 1;
}

void Nrf24Read(Nrf24Radio* Radio, uint8_t* data, uint8_t length)
{
	uint8_t blankLen = 0;
	if(Radio->dynamicPayloadsEnabled == 0)
	{
		length = min(length, Radio->payloadSize);
		blankLen = Radio->payloadSize - length;
	}
	else
	{
		length = min(length, 32);
	}
	
	PinSet(Radio->csnPin, LOW);
	
	SpiSend(R_RX_PAYLOAD);
	while(length--)
	{
		*data++ = SpiSend(NOP_CMD);
	}
	
	while(blankLen--)
	{
		SpiSend(NOP_CMD);
	}
	
	PinSet(Radio->csnPin, HIGH);
	
	WriteRegister(Radio, STATUS_ADDRESS, (1<<RX_DR));
}

uint8_t Nrf24Available(Nrf24Radio* Radio)
{
	uint8_t pipe = (GetStatus(Radio) >> RX_P_NO_0) & 0x07;
	return (pipe > 5)? 0 : 1;
}

uint8_t Nrf24RxFifoEmpty(Nrf24Radio* Radio)
{
	return ReadRegister(Radio, FIFO_STATUS_ADDRESS) & (1<<RX_EMPTY);
}