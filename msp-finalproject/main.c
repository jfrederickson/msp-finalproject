#include <msp430.h>
#include "servo.h"
#include "DCMotor.h"
#include "time.h"
#include "uart.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //Run initialization
    servoInit();
    DCMotorInit();
    initUART();
    setDCMotorSpeed(80,80);

    while(1){
    	//setDCMotorSpeed(-50,-50);
    	//delay(100000);
    	//setDCMotorSpeed(0,0,0,0);
    	//delay(1);
    }

}
