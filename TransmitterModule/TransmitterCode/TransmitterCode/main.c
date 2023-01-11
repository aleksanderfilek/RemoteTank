/*
 * Transmitter.c
 *
 * Created: 09.08.2022 16:30:30
 * Author : filek
 */ 

#include "uart.h"
#include "avr/interrupt.h"
#include "audio.h"
#include "commands.h"
#include "nrf24.h"
#include <stdlib.h>
#include "spi.h"


const uint64_t pipe = 0xE8E8F0F0E1LL;
Nrf24Radio* radioRef;
CommandManager* commandManagerRef;

void CallbackUart(uint8_t* data, uint16_t length);
void OnConnect(CommandType Type, CommandStatus Status, uint8_t* data);
void OnMotorControl(CommandType Type, CommandStatus Status, uint8_t* data);
void OnGetData(CommandType Type, CommandStatus Status, uint8_t* data);

int main(void)
{		
	CommandManager commandManager;
	commandManagerRef = &commandManager;
	
	CommandInit(commandManagerRef);
	CommandRegisterFunc(commandManagerRef, CMD_CONNECT, OnConnect);
	CommandRegisterFunc(commandManagerRef, CMD_MOTORCONTROL, OnMotorControl);
	CommandRegisterFunc(commandManagerRef, CMD_GETDATA, OnGetData);

	UartInit(9600, 0);
	UartCallbackSet(CallbackUart, COMMAND_LENGTH);
	
	SpiMasterInit();
	
	Nrf24Radio radio;
	radioRef = &radio;
	Nrf24Init(radioRef);
	sei();
	
	uint8_t quit = 0;
	
	if(Nrf24Begin(radioRef) == 0)
	{
		quit = 1;
	}
	
	Nrf24OpenWritingPipe(radioRef, pipe);
	AudioInit();
	
    while (quit == 0)
    {
    }
		
	CommandDestroy(commandManagerRef);
}

void CallbackUart(uint8_t* data, uint16_t length)
{
	CommandProcess(commandManagerRef, data, length);
}

void OnConnect(CommandType Type, CommandStatus Status, uint8_t* data)
{
	AudioBeep(1);
	if(Nrf24Write(radioRef, data, COMMAND_LENGTH) == 0)
	{
		data[1] = CMDS_FAILED;
		UartArraySend(data, COMMAND_LENGTH);
		return;
	}
	data[1] = CMDS_SUCCESS;
	UartArraySend(data, COMMAND_LENGTH);
	AudioBeep(1);
}

void OnMotorControl(CommandType Type, CommandStatus Status, uint8_t* data)
{
	if(Nrf24Write(radioRef, data, COMMAND_LENGTH) == 0)
	{
		data[1] = CMDS_FAILED;
		UartArraySend(data, COMMAND_LENGTH);
		return;
	}
	data[1] = CMDS_SUCCESS;
	UartArraySend(data, COMMAND_LENGTH);
}

void OnGetData(CommandType Type, CommandStatus Status, uint8_t* data)
{
	if(Nrf24Write(radioRef, data, COMMAND_LENGTH) == 0)
	{
		data[1] = CMDS_FAILED;
		UartArraySend(data, COMMAND_LENGTH);
		return;
	}
	
	Nrf24OpenReadingPipe(radioRef, pipe);
	Nrf24StartListening(radioRef);
	
	uint16_t timeoutCounter = 0;
	while(Nrf24Available(radioRef) == 0)
	{
		timeoutCounter++;
		if(timeoutCounter >= 1600)
		{
			data[1] = CMDS_FAILED;
			Nrf24StopListening(radioRef);
			Nrf24OpenWritingPipe(radioRef, pipe);
			UartArraySend(data, COMMAND_LENGTH);
			return;
		}
	}
	uint8_t receivedMsg[COMMAND_LENGTH];
	Nrf24Read(radioRef, receivedMsg, COMMAND_LENGTH);
	
	Nrf24StopListening(radioRef);
	Nrf24OpenWritingPipe(radioRef, pipe);
	
	UartArraySend(receivedMsg, COMMAND_LENGTH);
}