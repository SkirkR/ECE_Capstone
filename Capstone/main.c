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
    float curMax = 0.0f;
    lcd_init();
    init_sensor();
    while(1){
//        WAIT_FLICKER;
        curWeight = readWeight();
        char weightBuffer[20];
        lcd_init();
        if (curWeight > 5.0f) {
            if (curWeight > curMax) {
                curMax = curWeight;
            }
//            float offsetPercent = 1.00f;
//            if(curMax < 15.0f)  {
//                offsetPercent = 1.00f;
//            } else if(curMax < 20.0f)  {
//                offsetPercent = 1.01f;
//            } else if(curMax < 25.0f)  {
//                offsetPercent = 1.02f;
//            } else if(curMax < 30.0f)  {
//                offsetPercent = 1.04f;
//            } else if(curMax < 40.0f)   {
//                offsetPercent = 1.08f;
//            } else if(curMax < 50.0f)   {
//                offsetPercent = 1.10f;
//            } else if(curMax < 75.0f)   {
//                offsetPercent = 1.12f;
//            }
            ftoa((curMax), weightBuffer, 1);
//            ftoa((curMax*offsetPercent), weightBuffer, 1);
//            ftoa((curWeight), weightBuffer, 1);
            send_string("Weight: ");
            send_string(weightBuffer);
            send_string(" lbs    "); //Need spaces to get rid of cursor
        }   else    {
            send_string("  Lift Handle!           ");
            curMax = 0.0f;
        }
    }
}
