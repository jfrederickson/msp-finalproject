/*
 * DCMotor.c
 *
 * Author: James Ritchie III, Mario DeSantis, Jon Frederickson
 * Date Submitted: December 16, 2014
 * Latest Version: 20141216
 *
 * Sets up and controls two DC Motors using Timer A1
 * for PWM and 4 GPIO pins.
 *
 * The enable pins can take a PWM signal, which allows
 * for the control of the speed of the DC Motors. The
 * other 4 GPIO pins are used for controlling the
 * direction of the motors.
 *
 * This module sets up the pins needed for the operation
 * of two DC Motors on a L298 Dual H-Bridge Driver.
 * Pins 1.4, 1.5, 1.6, and 1.7 were used as the GPIO pins
 * to control direction, while Pins 2.2 and 2.5 were used
 * for controlling the 2 PWM signals to the enable pins on
 * the L298.
 */

#include "DCMotor.h"
#include <msp430.h>
#include "msp430_launchpad.h"

/**
 * Initializes the pins used for the DC Motors.
 * This sets Pins 2.2 and 2.5 up for PWM using
 * Timer A1 on the MSP430. Also sets up pins
 * 1.4 and 1.5 to control the input pins of
 * motor A, and 1.6 and 1.7 pins to control the
 * input pins of motor B.
 */
void DCMotorInit(void){
	//Set the configuration for Timer_A to Up Mode with SMCLK as source
	TA1CTL |= TASSEL_2 + MC_1;
	TA1CCTL1 |= OUTMOD_7;
	TA1CCTL2 |= OUTMOD_7;

	//Set up pins for input into DC Motor Driver.
	//This either need to be high or low to enable
	//current out in the corresponding output pin
	P1DIR |= PIN4 + PIN5 + PIN6 + PIN7;

	//Set up pins for Enable. Enable pins on
	//driver enable the set of pins to work.
	//Also set up so PWM can be outputted
	//through these pins.
	P2DIR |= PIN2 + PIN5;
	P2SEL |= PIN2 + PIN5;

	//Set TA1CCR0 so that our period is 20 ms
	TA1CCR0 = 22000;

	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR1 = 0;
	//Set TA1CCR1 to have an initial duty cycle
	TA1CCR2 = 0;
}

/**
 * Sets the motor speed and direction based on the input
 * for both motors. A negative value indicates a backwards
 * direction. This function will enable the correct input
 * pins for motor direction, while disabling the other
 * input pins. It also sends a PWM signal to the enable
 * pins to control speed of the motor.
 *
 * @param motorASpeed - int value from -128 to 127
 * @param motorBSpeed = int value from -128 to 127
 */
void setDCMotorSpeed(int8_t motorASpeed, int8_t motorBSpeed){
	//Both motors going forwards
	if(motorASpeed > 0 && motorBSpeed > 0){
		//Set reverse pins low
		P1OUT &= (~PIN5 + ~PIN6);
		//Set forwards pins high
		P1OUT |= PIN4 + PIN7;

		//Set duty cycle for the enable pins.
		//This controls how fast the motors go.
		//Boundary condition check
		if(motorASpeed > maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = 173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed > maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = 173*motorBSpeed;
		}
	}
	//Both motors going backwards
	else if(motorASpeed < 0 && motorBSpeed < 0){
		//Set forward pins low
		P1OUT &= (~PIN4 + ~PIN7);
		//Set backwards pins high
		P1OUT |= PIN5 + PIN6;

		//Set duty cycle for the enable pins.
		//Boundary condition check
		if(motorASpeed < -maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = -173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed < -maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = -173*motorBSpeed;
		}

	}
	//If motorA is going backwards and motorB is
	//going forwards.
	else if(motorASpeed < 0 && motorBSpeed > 0){
		//Turn off forward pin for motor A
		//Turn off backward pin for motor B
		P1OUT &= (~PIN4 + ~PIN6);
		//Turn on forward pin for motor B
		//Turn on backward pin for motor A
		P1OUT |= PIN5 + PIN7;

		//Set duty cycle for the enable pins.
		//Boundary condition check
		if(motorASpeed < -maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = -173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed > maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = 173*motorBSpeed;
		}
	}
	//If motorA is going forward and motorB is
	//going backwards
	else if(motorASpeed > 0 && motorBSpeed < 0){
		//Turn off forward pin for motor A
		//Turn off backward pin for motor B
		P1OUT &= (~PIN5 + ~PIN7);
		//Turn on forward pin for motor B
		//Turn on backward pin for motor A
		P1OUT |= PIN4 + PIN6;

		//Set duty cycle for the enable pins.
		//This controls how fast the motors go.
		//Boundary condition check
		if(motorASpeed > maxSpeed){
			TA1CCR1 = 173*maxSpeed;
		}
		else{
			TA1CCR1 = 173*motorASpeed;
		}
		//Boundary condition check
		if(motorBSpeed < -maxSpeed){
			TA1CCR2 = 173*maxSpeed;
		}
		else{
			TA1CCR2 = -173*motorBSpeed;
		}
	}
	else{ //Stop motors
		//Set all pins low
		P1OUT &= (~PIN4 + ~PIN5 + ~PIN6 + ~PIN7);

		//Set a duty cycle of 0 for enable pins
		TA1CCR1 = 0;
		TA1CCR2 = 0;
	}

}

void delay(uint32_t number){
	while(number--){
		//for(i = 0; i < 160000; i++);
	}
}


