/*
 * audio.c
 *
 * Created: 28.12.2022 19:37:19
 *  Author: filek
 */ 

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>

void AudioInit()
{
	DDRC |= (1<<DDC4);
	PORTC &= ~(1<<DDC4);
}

void AudioBeep(int times)
{
	for(int i = 0; i < times; i++)
	{
		PORTC |= (1<<DDC4);
		_delay_ms(200);
		PORTC &= ~(1<<DDC4);
		_delay_ms(100);
	}
}