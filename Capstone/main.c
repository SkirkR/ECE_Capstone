#include <msp430g2553.h>
#include "lcd.h"
#include "weigher.h"

void ConfigureClockModule(void){
    // Configure Digitally Controlled Oscillator (DCO) for 1 MHz using factory
    // calibrations.
	DCOCTL  = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
}

void main(void){
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
	ConfigureClockModule();
    float curWeight;
    while(1){
        curWeight = readWeight();
        if(curWeight > 3)   {
            lcd_init();
            send_string("Current Weight: ");
            char weightBuffer[64];
            snprintf(weightBuffer, sizeof weightBuffer, "%f", curWeight);
            send_string(weightBuffer);
            send_string(" pounds");
        }   else    {
            lcd_init();
        }
    }
}
