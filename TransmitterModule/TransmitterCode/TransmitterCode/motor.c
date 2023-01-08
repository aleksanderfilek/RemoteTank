
/*
 * pwm.c
 *
 * Created: 28.08.2022 18:16:11
 *  Author: filek
 */ 
#include "motor.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile static int factor[2];
volatile static int nextFactor[2];
void uint8_t direction[2];

ISR (TIMER0_COMPB_vect)
{
	static int counter = 0;
	counter++;

	uint8_t high = PORTD & (1<<DDD5);
	
	uint8_t realFactor = (high) ? factor[1] : 256 -  factor[1];
	if(counter == realFactor)
	{
		if(factor[1] != nextFactor[1])
		{
			factor[1] = nextFactor[1];
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
	
	uint8_t realFactor = (high) ? factor[0] : 256 -  factor[0];
	if(counter == realFactor)
	{
		if(factor[0] != nextFactor[0])
		{
			factor[0] = nextFactor[0];
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
	DDRD |= (1<<DDD6) | (1<<DDD5) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);

	factor[0] = factor[1] = 0;
	MotorSpeedSet(0, 0);
	MotorSpeedSet(1, 0);
	MotorDirectionSet(0, MD_STOP);
	MotorDirectionSet(1, MD_STOP);
}

void MotorSpeedSet(uint8_t motorId, uint8_t value)
{
	nextFactor[motorId] = value;
}

void MotorDirectionSet(uint8_t motorId, MotorDirection direction)
{
	uint8_t pin0 = DDD0 + 2*motorId;
	uint8_t pin1 = pin0 + 1;
	switch(direction)
	{
		case MD_STOP:
			PORTD &= ~(1<<pin0);
			PORTD &= ~(1<<pin1);
			break;
		case MD_FORWARD:
			PORTD |= (1<<pin0);
			PORTD &= ~(1<<pin1);
			break;
		case MD_BACKWARD:
			PORTD &= ~(1<<pin0);
			PORTD |= (1<<pin1);
			break;
	}
}