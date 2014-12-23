/*
 * servo.h
 *
 *  Created on: Sep 24, 2014
 *      Author: James Ritchie
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define DUTY_LOWER_BOUND 1100
#define DUTY_UPPER_BOUND 2200

void servoInit(void);
void servoSetDuty(uint16_t regVal);


#endif /* SERVO_H_ */
