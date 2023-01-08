/*
 * commands.h
 *
 * Created: 2022-12-29 00:02:59
 *  Author: Alek
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "config.h"

#define COMMAND_LENGTH 32

typedef enum CommandStatus
{
	CMDS_NONE = 0,
	CMDS_SUCCESS = 1,
	CMDS_FAILED = 2
} CommandStatus;

typedef enum CommandType
{
	CMD_NONE = 0,
	CMD_CONNECT = 1,
	CMD_MOTORCONTROL = 2,
	CMD_GETDATA = 3
} CommandType;

typedef void (*CommandFunc)(CommandType Type, CommandStatus Status, uint8_t* data);


typedef struct CommandRow
{
	CommandType Type;
	CommandFunc Func;
}CommandRow;

typedef struct CommandManager
{
	CommandRow* funcs;
	uint8_t count;
}CommandManager;

void CommandInit(CommandManager* Manager);
void CommandDestroy(CommandManager* Manager);
void CommandRegisterFunc(CommandManager* Manager, CommandType Type, CommandFunc Func);
void CommandProcess(CommandManager* Manager, uint8_t* data, uint16_t length);

#endif /* COMMANDS_H_ */