#include <msp430g2553.h>
#define DATAWRITE P2OUT |= BIT3 // define RS high
#define CMDWRITE P2OUT &= ~BIT3 // define RS low
#define READ P2OUT |= BIT5 // define Read signal R/W = 1 for reading
#define WRITE P2OUT &= ~BIT5 // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT |= BIT7 // define Enable high signal
#define ENABLE_LOW P1OUT &= ~BIT7 // define Enable Low signal
#define LONG_WAIT __delay_cycles(50000) //Wait 50ms since clock is 1Mhz
#define WAIT __delay_cycles(5000) //Wait 5ms since clock is 1Mhz
//In 4-bit operation upper 4 is sent, then lower 4

struct PinLayout data_to_pins(unsigned char data)	{
	//Mapping bits of data to correct pins
	//Only uses bottom 4 bits, as in 4bit mode
	struct PinLayout activePins;
	activePins.DirOne = 0x00;
	activePins.DirTwo = 0x00;
	if(data&0x01 != 0x00) activePins.DirTwo |= BIT1; //Mapping lowest to P2DIR,BIT1
	if(data&0x02 != 0x00) activePins.DirOne |= BIT4; //Mapping second lowest to P1DIR,BIT4
	if(data&0x04 != 0x00) activePins.DirOne |= BIT3; //Mapping second highest to P1DIR,BIT3
	if(data&0x08 != 0x00) activePins.DirOne |= BIT0; //Mapping highest to P1DIR,BIT0
	return activePins;
}

void clear_data_pins(void)	{
	P1OUT &= ~(BIT0 | BIT3 | BIT4);
	P2OUT &= ~(BIT1);
}
void data_comm(void){
    ENABLE_HIGH;
    WAIT;
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

void set_data_pins(unsigned char data)  {
    //Must send upper 4 bits, then lower 4 bits
    clear_data_pins; //Reset all data pins to 0
	struct PinLayout lowerNibble = data_to_pins(data);
	struct PinLayout upperNibble = data_to_pins(data>>4);
    P1OUT |= upperNibble.DirOne; // send higher nibble
	P2OUT |= upperNibble.DirTwo;
    data_comm(); // give enable trigger
    clear_data_pins; //Reset all data pins to 0
    P1OUT |= lowerNibble.DirOne; // send lower nibble
	P2OUT |= lowerNibble.DirTwo;
    data_comm(); // give enable trigger
    clear_data_pins;
}

void send_command(unsigned char cmd){
    //check_busy();
    WRITE; //Sets R/W bit to low to indicate writing
    CMDWRITE; //Sets RS to low to indicate command
    set_data_pins(cmd);
}

void send_data(unsigned char data){
    //check_busy();
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
    P1DIR |= BIT0; //DataPin 7
	P1DIR |= BIT3; //DataPin 6
	P1DIR |= BIT4; //DataPin 5
	P2DIR |= BIT1; //DataPin 4
	P1DIR |= BIT7; //Enable pins
	P2DIR |= BIT3; //Register Select(Command or Data)
	P2DIR |= BIT5; //Read/Write
    P1OUT &= ~(BIT0 | BIT3 | BIT4 | BIT7); //All outputs are low
	P2OUT &= ~(BIT1 | BIT3 | BIT5);
    LONG_WAIT; //50ms wait at powerup
	for(i = 0; i < 3; i++)	{//Send wakeup/4-bit 3 times
		send_command(0x22);
		WAIT;
	}
    send_command(0x28); //4-bit/2-line
    WAIT;
	send_command(0x10); //Set Cursor
    WAIT;
    send_command(0x0F); //Display on, cursor blinking
    WAIT;
    send_command(0x06); //Setup entry mode
    WAIT;
}