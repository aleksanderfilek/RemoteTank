
/*
 * pwm.c
 *
 * Created: 28.08.2022 18:16:11
 *  Author: filek
 */ 
#include "motor.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile static int factor;
volatile static int nextFactor;
ISR (TIMER0_COMPB_vect)
{
	static int counter = 0;
	counter++;

	uint8_t high = PORTD & (1<<DDD5);
	
	uint8_t realFactor = (high) ? factor : 256 -  factor;
	if(counter == realFactor)
	{
		if(factor != nextFactor)
		{
			factor = nextFactor;
		}
		PORTD ^= (1<<DDD5);
		counter = 0;
	}
}
ISR (TIMER0_COMPA_vect)
{
	static int counter = 0;
	counter++;

	uint8_t high = PORTD & (1<<DDD6);
	
	uint8_t realFactor = (high) ? factor : 256 -  factor;
	if(counter == realFactor)
	{
		if(factor != nextFactor)
		{
			factor = nextFactor;
		}
		PORTD ^= (1<<DDD6);
		counter = 0;
	}
}

void MotorInit()
{
	TCCR0A = (1<<WGM01);
	TCCR0B = (1<<CS00) | (1<<CS01);
	TIMSK0 = (1<<OCIE0A);
	OCR0A = 5;
	OCR0B = 5;
	DDRD |= (1<<DDD6) | (1<<DDD5);


	factor = 255;
	nextFactor = factor;
}

void MotorSetValue(uint8_t value)
{
	nextFactor = value;
}
