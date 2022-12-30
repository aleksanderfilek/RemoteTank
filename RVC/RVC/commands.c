/*
 * commands.c
 *
 * Created: 30.12.2022 23:23:08
 *  Author: filek
 */ 

#include "commands.h"
#include <stdlib.h>

void CommandInit(CommandManager* Manager)
{
	Manager->funcs = NULL;
	Manager->count = 0;
}

void CommandDestroy(CommandManager* Manager)
{
	if(Manager->funcs != NULL)
	{
		free(Manager->funcs);
		Manager->funcs = NULL;
	}
	Manager->count = 0;	
}

void CommandRegisterFunc(CommandManager* Manager, CommandType Type, CommandFunc Func)
{
	Manager->count++;
	Manager->funcs = (CommandRow*)realloc(Manager->funcs, Manager->count * sizeof(CommandRow));
	Manager->funcs[Manager->count - 1] = (CommandRow){ Type, Func };
}

void CommandProcess(CommandManager* Manager, uint8_t* data, uint16_t length)
{
	for(int i = 0; i < Manager->count; i++)
	{
		if(Manager->funcs[i].Type == data[0])
		{
			Manager->funcs[i].Func(data[0], data[1], &data[2]);
			return;
		}
	}
}