/*
 * motor.h
 *
 * Created: 28.08.2022 18:16:20
 *  Author: filek
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "config.h"

typedef enum MotorDirection
{
	MD_STOP = 0,
	MD_FORWARD = 1,
	MD_BACKWARD = 2
}MotorDirection;

void MotorInit();
void MotorSpeedSet(uint8_t motorId, uint8_t value);
void MotorDirectionSet(uint8_t motorId);

#endif /* PWM_H_ */