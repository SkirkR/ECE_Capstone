#include <msp430g2553.h>
#include "weigher.h"

#define WAIT_50MS __delay_cycles(5000)
#define WAIT_5MS __delay_cycles(500)
#define READ_CLOCK (P1IN&BIT5)
#define READ_SDO (P1IN&BIT6)
#define SELECT_HIGH P2OUT |= BIT0;
#define SELECT_LOW P2OUT &= ~BIT0;

long get_data(void) {
    SELECT_LOW;
    long sampleData = 0x0000;
    while(READ_CLOCK != 0x00);
    while(READ_CLOCK == 0x00);
    if(READ_SDO != 0) { //First bit is push on rising edge
        sampleData |= 0x01;
    }
    int i = 0;
    for(;i < 31; i++)   {
        sampleData = sampleData << 1;
        while(READ_CLOCK != 0x00);
        if(READ_SDO != 0)   {
            sampleData |= 0x01;
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
    SELECT_HIGH; //CS High
}

float readWeight(void)  {
    float avgWeight = 0.0f;
    int i = 0;
    for(;i < 5; i++)    {
        long sampleData = get_data();//Guaranteed to be correct
        if((sampleData&0x20000000) == 0) {
            sampleData = 0x00;
        }
        long maxValue = (0x00800000);
        sampleData >>= 5;
        sampleData &= 0x00FFFFFF;
        float fractionalOutput = ((float)sampleData)/((float)maxValue);
        float curVoltage = fractionalOutput*2.5f;
        float curWeight = curVoltage*1000.0f;
        avgWeight += curWeight;
    }
    avgWeight /= 5.0f;
    return avgWeight;
}
