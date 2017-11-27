
// This file is for 4 bit mode LCD interfacing with msp430g2553 chip
// 16x2 LCD is used
#include <msp430g2553.h>
#include "lcd.h"

void ConfigureClockModule(void){
    // Configure Digitally Controlled Oscillator (DCO) for 1 MHz using factory
    // calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void main(void){
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
	ConfigureClockModule();
    lcd_init();
    send_string("321 TEST 123");
    while(1){
    }
}
