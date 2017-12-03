#include <msp430g2553.h>
#include "weigher.h"

#define WAIT_50MS __delay_cycles(5000)
#define WAIT_5MS __delay_cycles(500)
#define READ_CLOCK (P1IN&BIT5)
#define READ_SDO (P1IN&BIT6)
#define SELECT_HIGH P2OUT |= BIT0;
#define SELECT_LOW P2OUT &= ~BIT0;
#define TESTBIT_HIGH P2OUT |= BIT4;
#define TESTBIT_LOW P2OUT &= ~BIT4;

long get_data(void) {
    SELECT_LOW;
    long sampleData = 0x0000;
    while(READ_CLOCK != 0x00);
    while(READ_CLOCK == 0x00);
    if(READ_SDO != 0) { //First bit is push on rising edge
        sampleData |= 0x01;
        TESTBIT_HIGH;
    }
    int i = 0;
    for(;i < 31; i++)   {
        sampleData = sampleData << 1;
        while(READ_CLOCK != 0x00);
        if(READ_SDO != 0)   {
            sampleData |= 0x01;
            TESTBIT_HIGH;
        }   else    {
            TESTBIT_LOW;
        }
        while(READ_CLOCK == 0x00);
    }
    SELECT_HIGH;
    return sampleData;
}

void init_sensor(void)  {
    P1DIR &= ~BIT5; //SCK
    P2DIR |= BIT0; //CS
    P1DIR &= ~BIT6; //SDO
    P2DIR |= BIT4;
    SELECT_HIGH; //CS High
}

float readWeight(void)  {
    long sampleData = get_data();//Guaranteed to be random
    volatile long mask = 1;
    sampleData = sampleData&(~(mask<<31));
    float curWeight = (float)sampleData;
    return curWeight;
}
