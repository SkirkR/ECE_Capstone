#include <msp430g2553.h>
#include "lcd.h"
//1.2 is contrast 0.7V is optimum
#define DATAWRITE P2OUT |= BIT3 // define RS high
#define CMDWRITE P2OUT &= ~BIT3 // define RS low
#define READ P2OUT |= BIT5 // define Read signal R/W = 1 for reading
#define WRITE P2OUT &= ~BIT5 // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT |= BIT7 // define Enable high signal
#define ENABLE_LOW P1OUT &= ~BIT7 // define Enable Low signal
#define DB7_AS_INPUT P1DIR &= ~BIT0 //Sets DB7 as an input
#define READ_DB7 (P1IN&BIT0) //Reads DB7
#define DB7_AS_OUTPUT P1DIR |= BIT0 //Sets DB7 as an output
#define DB7_ACTIVE P1OUT |= BIT0 //Turn on DB7
#define DB7_INACTIVE P1OUT &= ~BIT0 //Turn off DB7
#define DB6_ACTIVE P1OUT |= BIT3 //Turn on DB6
#define DB6_INACTIVE P1OUT &= ~BIT3 //Turn off DB6
#define DB5_ACTIVE P1OUT |= BIT4 //Turn on DB5
#define DB5_INACTIVE P1OUT &= ~BIT4 //Turn off DB5
#define DB4_ACTIVE P2OUT |= BIT1 //Turn on DB4
#define DB4_INACTIVE P2OUT &= ~BIT1 //Turn off DB4
#define WAIT_50MS __delay_cycles(5000)
#define WAIT_5MS __delay_cycles(500)
//BIT3 is DB7, BIT0 is DB3
//In 4-bit operation upper 4 is sent, then lower 4

void data_comm(void){
    ENABLE_HIGH;
    ENABLE_LOW;
}
void check_busy(void){
    READ;
    DB7_AS_INPUT; //Looks at DB7 as busy bit
    while(READ_DB7 == 0){
        data_comm(); //Asks for busy status again
    }
    DB7_AS_OUTPUT; //Sets DB7 back to an output
}
void send_nibble(unsigned char nibble)    {
    DB7_INACTIVE;
    DB6_INACTIVE;
    DB5_INACTIVE;
    DB4_INACTIVE;
    if((nibble&0x01) != 0x00) DB4_ACTIVE; //Mapping lowest to P2DIR,BIT1
    if((nibble&0x02) != 0x00) DB5_ACTIVE; //Mapping second lowest to P1DIR,BIT4
    if((nibble&0x04) != 0x00) DB6_ACTIVE; //Mapping second highest to P1DIR,BIT3
    if((nibble&0x08) != 0x00) DB7_ACTIVE; //Mapping highest to P1DIR,BIT0
    data_comm(); // give enable trigger
}
void set_data_pins(unsigned char data)  {
    //Must send upper 4 bits, then lower 4 bits
    send_nibble(data>>4);
    send_nibble(data);
}
void send_command(unsigned char cmd){
    check_busy();
    WRITE; //Sets R/W bit to low to indicate writing
    CMDWRITE; //Sets RS to low to indicate command
    set_data_pins(cmd);
    WAIT_5MS;
}
void send_data(unsigned char data){
    check_busy();
    WRITE; //Sets R/W bit to low to indicate writing
    DATAWRITE; //Sets RS to high to indicate data
    set_data_pins(data);
    WAIT_5MS;
}
void send_string(char* s){
    while(*s){
        send_data((*s));
        s++;
    } //Sends data one char at a time from array
}
void lcd_init(void){
    P1DIR |= BIT0 | BIT3 | BIT4 | BIT7; //These pins are outputs
    P2DIR |= BIT1 | BIT3 | BIT5;
    WAIT_50MS;
    send_command(0x30); //Wakeup command
    WAIT_5MS;
    send_command(0x28); //4 bit mode
    send_command(0x10); //Clear display
    send_command(0x01); //Clear display
    send_command(0x0F); //Display on, cursor blinking
    send_command(0x06); //Setup entry mode
}
