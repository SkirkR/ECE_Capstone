#include <msp430g2553.h>
#include "floatConv.h"
#include "lcd.h"
#include "weigher.h"
#define WAIT_FLICKER __delay_cycles(500000)

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
    lcd_init();
    init_sensor();
    while(1){
        WAIT_FLICKER;
        curWeight = readWeight();
        char weightBuffer[20];
        ftoa(curWeight, weightBuffer, 1);
        lcd_init();
        if (weightBuffer[0] != '.') {
            send_string("Weight: ");
            send_string(weightBuffer);
            send_string(" lbs    "); //Need spaces to get rid of cursor
        }   else    {
            send_string("Lift Handle!           ");
        }
    }
}
