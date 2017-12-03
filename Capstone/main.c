#include <msp430g2553.h>
#include "floatConv.h"
#include "lcd.h"
#include "weigher.h"
#define WAIT_2S __delay_cycles(2000000)

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
        WAIT_2S;
        curWeight = readWeight();
//        if(curWeight > 3.0f)   {
            lcd_init();
            send_string("Weight: ");
            char weightBuffer[20];
            ftoa(curWeight, weightBuffer, 1);
            send_string(weightBuffer);
            send_string(" lbs");
//        }   else    {
//            lcd_init();
//        }
    }
}
