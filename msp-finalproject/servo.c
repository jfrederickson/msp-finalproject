/*
 * servo.c
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 *
 * Sets up and controls a servo motor using pin 2.6 and Timer A0.
 */

#include "servo.h"
#include <msp430.h>
#include "msp430_launchpad.h"

/**
 * Initializes servo, setting up pin 2.6 to be used for PWM output
 * to the servo. Also sets the default start state for the servo.
 */
void servoInit(void){
	//Set the configuration for Timer_A2 to Up Mode with SMCLK as source, interrupts enabled
	TA0CTL |= TASSEL_2 + MC_1;
	TA0CCTL1 |= OUTMOD_7;

	//Configure P2.6 to use primary peripheral
	P2DIR |= PIN6;
	P2SEL |= PIN6;
	//Must disable XOUT pin (2.7) to use P2.6 for timer.
	P2SEL &= ~PIN7;

	//Set TACCR0 so that our period is 20 ms
	TA0CCR0 = 22000;

	//Set TA1CCR1 to have an initial period
	TA0CCR1 = 1650;
}

/*
 * Sets the direction of the servo.
 *
 * @param regVal - Value to set the servo to
 */
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
