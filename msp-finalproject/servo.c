/*
 * servo.c
 *
 *  Created on: Sep 24, 2014
 *      Author: James Ritchie
 *      Author: Jonathan Frederickson
 */

#include "servo.h"
#include <msp430.h>
#include "msp430_launchpad.h"

void servoInit(void){
	//Set the configuration for Timer_A2 to Up Mode with SMCLK as source, interrupts enabled
	TA0CTL |= TASSEL_2 + MC_1;
	TA0CCTL1 |= OUTMOD_7;

	//Configure P1.6 to use primary peripheral
	P2DIR |= PIN6;
	P2SEL |= PIN6;
	P2SEL &= ~PIN7;

	//Set TACCR0 so that our period is 20 ms
	TA0CCR0 = 22000;

	//Set TA1CCR1 to have an initial period
	TA0CCR1 = 1650;
}

void servoSetDuty(uint16_t regVal){
	if(regVal >= DUTY_UPPER_BOUND){
		TA0CCR1 = DUTY_UPPER_BOUND;
	}
	else if(regVal <= DUTY_LOWER_BOUND){
		TA0CCR1 = DUTY_LOWER_BOUND;
	}
	else{
		TA0CCR1 = regVal;
	}
}

void servoSetDirection(int8_t dir) {
	if(dir > 0) {
		servoSetDuty((DUTY_UPPER_BOUND/127) * dir);
	}
	else if(dir < 0) {
		servoSetDuty(-(DUTY_LOWER_BOUND/127) * dir);
	}
	else {
		// set to right in the middle
		servoSetDuty(DUTY_UPPER_BOUND - ((DUTY_UPPER_BOUND-DUTY_LOWER_BOUND)/2));
	}
}
