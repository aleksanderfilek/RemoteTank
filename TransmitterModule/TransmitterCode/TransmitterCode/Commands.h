/*
 * Commands.h
 *
 * Created: 2022-11-22 22:25:01
 *  Author: Alek
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_

#define COMMAND_LENGTH 32

typedef enum CommandType
{
	CMD_MOVE = 1,
	CMD_STOP,
	CMD_INFO_REQ,
	CMD_INFO_RSP,
	ERR_RADIO_NOT_STARTED
} CommandType;

#endif /* COMMANDS_H_ */