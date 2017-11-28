#include <msp430g2553.h>
#include "lcd.h"
#define DATAWRITE P2OUT |= BIT3 // define RS high
#define CMDWRITE P2OUT &= ~BIT3 // define RS low
#define READ P2OUT |= BIT5 // define Read signal R/W = 1 for reading
#define WRITE P2OUT &= ~BIT5 // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT |= BIT7 // define Enable high signal
#define ENABLE_LOW P1OUT &= ~BIT7 // define Enable Low signal
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
    P1DIR &= ~BIT0; //Looks at DB7 as busy bit
    while((P1IN&BIT0) == 0){
        data_comm(); //Asks for busy status again
    }
    P1DIR |= BIT0; //Sets DB7 back to an output
}
void send_nibble(unsigned char nibble)    {
    P1OUT &= ~BIT0 & ~BIT3 & ~BIT4; //Reset all data pins to 0
    P2OUT &= ~BIT1;
    if((nibble&0x01) != 0x00) P2OUT |= BIT1; //Mapping lowest to P2DIR,BIT1
    if((nibble&0x02) != 0x00) P1OUT |= BIT4; //Mapping second lowest to P1DIR,BIT4
    if((nibble&0x04) != 0x00) P1OUT |= BIT3; //Mapping second highest to P1DIR,BIT3
    if((nibble&0x08) != 0x00) P1OUT |= BIT0; //Mapping highest to P1DIR,BIT0
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
