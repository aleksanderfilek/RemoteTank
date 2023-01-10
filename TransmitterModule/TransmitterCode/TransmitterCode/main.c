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
	uint8_t forward = data[0];
	uint8_t right = data[1];
	uint8_t speed[2] = { 0, 0 };
	MotorDirection direction = MD_STOP;
	if(forward + right > 0)
	{
		direction = (forward == 2)? MD_BACKWARD: MD_FORWARD;
		speed[0] = speed[1] = 255;
		
		if(right > 0)
		{
			uint8_t turnMotorId = right - 1;
			speed[turnMotorId] = (forward > 0)? 128: 0;
		}
	}
	MotorSpeedSet(0, speed[0]);
	MotorSpeedSet(1, speed[1]);
	MotorDirectionSet(0, direction);
	MotorDirectionSet(1, direction);
}

float map(long x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define MPU6050_ADDR 0b1101000

void OnGetData(CommandType Type, CommandStatus Status, uint8_t* data)
{
	uint8_t temperatureReg[2];
	TwiRead(MPU6050_ADDR, 0x41, temperatureReg, 2);
	int temperatureBig = (temperatureReg[0] << 7) | temperatureReg[1];
	float temperature = ((float)temperatureBig / 340.0f) + 36.53f;
	
	uint8_t accelerometerReg[6];
	TwiRead(MPU6050_ADDR, 0x3B, accelerometerReg, 6);
	float acX = (float)((accelerometerReg[0] << 8) | accelerometerReg[1]);
	float acY = (float)((accelerometerReg[2] << 8) | accelerometerReg[3]);
	float acZ = (float)((accelerometerReg[4] << 8) | accelerometerReg[5]);
	
	float mappedX = map(acX, 265, 402, -90.0f, 90.0f);
	float mappedY = map(acY, 265, 402, -90.0f, 90.0f);
	float mappedZ = map(acZ, 265, 402, -90.0f, 90.0f);

	float angleX = (atan2(-mappedY, -mappedZ) + M_PI) * 180.0f / M_PI;
	float angleY = (atan2(-mappedX, -mappedZ) + M_PI) * 180.0f / M_PI;
	float angleZ = (atan2(-mappedY, -mappedX) + M_PI) * 180.0f / M_PI;
	
	uint16_t newDataLength = 4 * sizeof(float);
	uint8_t newData[newDataLength];
	memcpy(newData, &temperature, sizeof(float));
	memcpy(newData + sizeof(float), &angleX, sizeof(float));
	memcpy(newData + 2*sizeof(float), &angleY, sizeof(float));
	memcpy(newData + 3*sizeof(float), &angleZ, sizeof(float));

	uint8_t* command = CommandCreate(CMD_GETDATA, CMDS_SUCCESS, newData, newDataLength);
	
	Nrf24StopListening(radioRef);
	Nrf24OpenWritingPipe(radioRef, pipe);
	Nrf24Write(radioRef, command, COMMAND_LENGTH);
	free(command);

	Nrf24OpenReadingPipe(radioRef, pipe);
	Nrf24StartListening(radioRef);
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

