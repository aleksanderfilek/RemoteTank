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

//
const uint64_t pipe = 0xE8E8F0F0E1LL;
Nrf24Radio* radioRef;
//
//void CallbackUart(uint8_t* data, uint16_t length)
//{
	//Nrf24Write(radio, data, length);
	//
	//uint8_t waitForResponse = data[1];
	//if(waitForResponse == 0)
		//return;
		//
	//Nrf24OpenReadingPipe(radio, pipe);
	//Nrf24StartListening(radio);
	//
	//while(Nrf24Available(radio) == 0){}
		//
	//uint8_t buffer[32];
	//Nrf24Read(radio, buffer, 32);
	//UartArraySend(buffer, 32);
	//
	//Nrf24StopListening(radio);
	//Nrf24OpenWritingPipe(radio, pipe);
//}

CommandManager* commandManagerRef;

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

void OnMotorControl(CommandType Type, CommandStatus Status, uint8_t* data)
{
	
}

void OnGetData(CommandType Type, CommandStatus Status, uint8_t* data)
{
	
}

int main(void)
{		
	//Nrf24Radio radio;
	//radioRef = &radio;
	//Nrf24Init(radioRef);
	
	CommandManager commandManager;
	commandManagerRef = &commandManager;
	
	CommandInit(commandManagerRef);
	CommandRegisterFunc(commandManagerRef, CMD_CONNECT, OnConnect);
	CommandRegisterFunc(commandManagerRef, CMD_MOTORCONTROL, OnMotorControl);
	CommandRegisterFunc(commandManagerRef, CMD_GETDATA, OnGetData);

	UartInit(9600, 0);
	UartCallbackSet(CallbackUart, COMMAND_LENGTH);
	//
	//SpiMasterInit();
	sei();
	//
	//if(Nrf24Begin(radio) == 0)
	//{
		//UartByteSend(ERR_RADIO_NOT_STARTED); // jakies zbieranie errrorowy by sie przydalo
		//return 0;
	//}
	//
	//Nrf24OpenWritingPipe(radioRef, pipe);
	AudioInit();
	
    while (1)
    {
	    //if(UartCountRead() > 0)
	    //{
		    //UartRead();
		    //AudioBeep(2);
		    //UartStringSend("Hello, App!\n");
	    //}
    }
		
	CommandDestroy(commandManagerRef);
}

