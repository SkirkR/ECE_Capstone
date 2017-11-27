#ifndef LCD_H_
#define LCD_H_

struct PinLayout	{
	unsigned char DirOne;
	unsigned char DirTwo;
};

void clear_data_pins(void);
void data_comm(void);
void check_busy(void);
void set_data_pins(unsigned char data);
void send_command(unsigned char cmd);
void send_data(unsigned char data);
void send_string(char* s);
void lcd_init(void);
struct PinLayout data_to_pins(unsigned char data);

#endif