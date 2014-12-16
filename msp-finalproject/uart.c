/* UART driver from http://www.argenox.com/library/msp430/ch8-msp430_uart.php */
#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include "servo.h"
#include "DCMotor.h"

int initUART(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    /* Use Calibration values for 1MHz Clock DCO*/
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    /* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
    P1SEL = BIT1 | BIT2 ;
    P1SEL2 = BIT1 | BIT2;

    /* Place UCA0 in Reset to be configured */
    UCA0CTL1 = UCSWRST;

    /* Configure */
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 104;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1

    /* Take UCA0 out of reset */
    UCA0CTL1 &= ~UCSWRST;

    /* Enable USCI_A0 RX interrupt */
    IE2 |= UCA0RXIE;

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

/*  Echo back RXed character, confirm TX buffer is ready first */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{

	while(!(IFG2 & UCA0TXIFG));
	uint8_t cmd = UCA0RXBUF;
	UCA0TXBUF = cmd;
//	while(!(IFG2 & UCA0TXIFG));
//	uint8_t len = UCA0RXBUF;
//	UCA0TXBUF = len;

	while(!(IFG2 & UCA0TXIFG));
	int16_t data = UCA0RXBUF;
	int8_t data_signed = (int8_t) data-128;
	UCA0TXBUF = data;

	// 0x44 is "D" - setting direction
	if(cmd == 0x44) {
		servoSetDirection(data);
	}
	// 0x53 is "S" - setting speed
	// TODO: convert to signed int
	else if(cmd == 0x53) {
		setDCMotorSpeed(data_signed, data_signed);
	}
	else {
		setDCMotorSpeed(80, 80);
		while(!(IFG2 & UCA0TXIFG));
		UCA0TXBUF = 0xff;
		while(!(IFG2 & UCA0TXIFG));
		UCA0TXBUF = 0xff;
	}
}
