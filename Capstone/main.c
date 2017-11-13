
// This file is for 4 bit mode LCD interfacing with msp430g2553 chip
// 16x2 LCD is used
#include <msp430g2553.h>
#define DATAWRITE P1OUT |= BIT4 // define RS high
#define CMDWRITE P1OUT &= ~BIT4 // define RS low
#define READ P1OUT |= BIT5 // define Read signal R/W = 1 for reading
#define WRITE P1OUT &= ~BIT5 // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT |= BIT6 // define Enable high signal
#define ENABLE_LOW P1OUT &= ~BIT6 // define Enable Low signal
//BIT3 is DB7, BIT0 is DB3
//In 4-bit operation upper 4 is sent, then lower 4

void delay(unsigned int k){
    unsigned int i,j;
    for(j = 0; j <= k; j++){
        for(i = 0; i < 1000; i++);
    }
}
void data_comm(void){
    ENABLE_HIGH;
    //__delay_cycles(1000);
    delay(30);
    ENABLE_LOW;
}
void check_busy(void){
    READ;
    P1DIR &= ~BIT3; //Looks at DB7 as busy bit
    while((P1IN&BIT3) == 0){
        data_comm(); //Asks for busy status again
    }
    P1DIR |= BIT3; //Sets DB7 back to an output
}
void set_data_pins(unsigned char data)  {
    //Must send upper 4 bits, then lower 4 bits
    P1OUT &= ~BIT0 & ~BIT1 & ~BIT2 & ~BIT3; //Reset all data pins to 0
    P1OUT |= (data>>4) & 0x0F; // send higher nibble
    data_comm(); // give enable trigger
    P1OUT &= ~BIT0 & ~BIT1 & ~BIT2 & ~BIT3; //Reset all data pins to 0
    P1OUT |= (data) & 0x0F; // send lower nibble
    data_comm(); // give enable trigger
    //P1OUT &= ~BIT0 & ~BIT1 & ~BIT2 & ~BIT3; //Reset all data pins to 0
}
void send_command(unsigned char cmd){
    check_busy();
    WRITE; //Sets R/W bit to low to indicate writing
    CMDWRITE; //Sets RS to low to indicate command
    set_data_pins(cmd);
}
void send_data(unsigned char data){
    check_busy();
    WRITE; //Sets R/W bit to low to indicate writing
    DATAWRITE; //Sets RS to high to indicate data
    set_data_pins(data);
}
void send_string(char* s){
    while(*s){
        send_data((*s));
        s++;
    } //Sends data one char at a time from array
}
void lcd_init(void){
    P1DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6; //These pins are outputs
    P1OUT &= 0x00; //All outputs are low
    __delay_cycles(10000);
    send_command(0x30); //Wakeup command
    __delay_cycles(10000);
//    send_command(0x33); //Wakeup command
//    __delay_cycles(10000);
    send_command(0x28); //4 bit mode
    __delay_cycles(10000);
    send_command(0x28); //4 bit/2 line
    __delay_cycles(10000);
    send_command(0x10); //Clear display
    __delay_cycles(10000);
    send_command(0x01); //Clear display
    __delay_cycles(10000);
    send_command(0x0F); //Display on, cursor blinking
    __delay_cycles(10000);
    send_command(0x06); //Setup entry mode
    __delay_cycles(10000);
}

void main(void){
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer
    lcd_init();
    send_string("321TEST123");
    while(1){
    }
}
