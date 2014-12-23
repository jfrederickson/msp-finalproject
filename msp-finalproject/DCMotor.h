/*
 * DCMotor.h
 *
 *  Created on: Dec 15, 2014
 *      Author: James
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include <stdint.h>

#define maxSpeed 127

void DCMotorInit(void);
void setDCMotorSpeed(int8_t motorASpeed, int8_t motorBSpeed);
void delay(uint32_t number);

#endif /* DCMOTOR_H_ */
