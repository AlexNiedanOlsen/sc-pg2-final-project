/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#define REPLACEME 0



int main(void) {
	button_init();
	lcd_init();
	timer_init();
	cyBot_uart_init();
	            // Don't forget to initialze the cyBot UART before trying to use it
	
	// YOUR CODE HERE
	

	while(1)
	{
	    lcd_clear();
	    char button = button_getButton();
	    if(button == 0) {
	        lcd_putc('0');
	    } else {
	        lcd_putc(button + '0');
	    }
	    if(button != 0) {
	        cyBot_sendByte(button + '0');
	    }
	    timer_waitMillis(200);
	
	}
	
}
