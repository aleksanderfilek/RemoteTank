/*
 * commands.h
 *
 * Created: 2022-12-29 00:02:59
 *  Author: Alek
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_


#define COMMAND_LENGTH 32

typedef enum CommandStatus
{
	CMDS_OK = 1,
	CMDS_FAILED = 2
} CommandStatus;

typedef enum CommandType
{
	CMD_CONN = 1
} CommandType;


#endif /* COMMANDS_H_ */