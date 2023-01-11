/*
 * VehicleCode.c
 *
 * Created: 2023-01-11 22:49:28
 * Author : Alek
 */ 


#include "config.h"
#include "motor.h"
#include "twi.h"
#include "nrf24.h"
#include "spi.h"
#include "commands.h"
#include "avr/interrupt.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MPU6050_ADDR 0b1101000

uint64_t pipe = 0xE8E8F0F0E1LL;
Nrf24Radio* radioRef;
CommandManager* commandManagerRef;

void OnMotorControl(CommandType Type, CommandStatus Status, uint8_t* data);
void OnGetData(CommandType Type, CommandStatus Status, uint8_t* data);

int main(void)
{
	CommandManager commandManager;
	commandManagerRef = &commandManager;
	
	CommandInit(commandManagerRef);
	CommandRegisterFunc(commandManagerRef, CMD_MOTORCONTROL, OnMotorControl);
	CommandRegisterFunc(commandManagerRef, CMD_GETDATA, OnGetData);
	
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
	
	Nrf24OpenReadingPipe(radioRef, pipe);
	Nrf24StartListening(radioRef);
	
	while (quit == 0)
	{
		if(Nrf24Available(radioRef) == 32)
		{
			uint8_t buffer[32];
			Nrf24Read(radioRef, buffer, COMMAND_LENGTH);
			CommandProcess(commandManagerRef, buffer, COMMAND_LENGTH);
		}
	}
	
	CommandDestroy(commandManagerRef);
}

float map(long x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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