/*
 * lcd.h
 *
 *  Created on: Nov 27, 2017
 *      Author: SKIrk
 */

#ifndef LCD_H_
#define LCD_H_

void data_comm(void);
void check_busy(void);
void send_nibble(unsigned char nibble);
void set_data_pins(unsigned char data);
void send_command(unsigned char cmd);
void send_data(unsigned char data);
void send_string(char* s);
void lcd_init(void);


#endif /* LCD_H_ */
