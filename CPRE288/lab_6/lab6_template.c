/**
 * lab6_template.c
 * 
 * Template file for CprE 288 lab 6
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include <math.h>
#include "open_interface.h"
#include "movement.h"


#include "cyBot_uart.h"  // Functions for communiticate between CyBot and Putty (via UART)
                         // PuTTy: Buad=115200, 8 data bits, No Flow Control, No Party,  COM1

#include "cyBot_Scan.h"  // For scan sensors 



#define REPLACEME 0


// Defined in button.c : Used to communicate information between the
// the interupt handeler and main.
extern volatile int button_event;
extern volatile int button_num;


int main(void) {
	button_init();
	lcd_init();
	init_button_interrupts();
    cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

	// Complete this code for configuring the  (GPIO) part of UART initialization
    SYSCTL_RCGCGPIO_R |= 0b10;
    timer_waitMillis(1);            // Small delay before accessing device after turning on clock
    GPIO_PORTB_AFSEL_R |= 0b11;
    GPIO_PORTB_PCTL_R &= ~0xFF;     // Force 0's in the disired locations
    GPIO_PORTB_PCTL_R |= 0x11;     // Force 1's in the disired locations
    GPIO_PORTB_DEN_R |= 0b11;
    GPIO_PORTB_DIR_R &= 0xFE;      // Force 0's in the disired locations
    GPIO_PORTB_DIR_R |= 0b10;      // Force 1's in the disired locataions
    
    cyBot_uart_init_last_half();  // Completes the UART device initialization part of configuration
	
/*
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0x00000007);
    cyBOT_SERVO_cal();
*/
	// Initialze scan sensors
    //cyBOT_init_Scan();



	// YOUR CODE HERE
    // part 2
    /*
	lcd_putc('0');
	while(1)
	{

	    lcd_putc(button_num + '0');

	    lcd_clear();
  

	    if(button_num == 0) {
	        lcd_putc('0');
	    } else {
	        lcd_putc(button_num + '0');
	    }
	    if(button_num != 0) {
	        cyBot_sendByte(button_num + '0');
	    }
	    timer_waitMillis(200);

	}

	// data collection for part 3
    //part 3
    // R 290500
    // L 1261750
    // CyBot 7

    timer_init();
    lcd_init();
    cyBOT_init_Scan(0x00000007);
    //cyBOT_SERVO_cal();
    right_calibration_value = 233750;
    left_calibration_value = 1267000;

    cyBOT_Scan_t scan;

    // header
    char line[100] = "PING\t\tIR\t\tScaled IR\n\r";


    int j;
    for(j = 0; line[j] != '\0'; j++) { // send header
        cyBot_sendByte(line[j]);
    }

// old code start here
    for(j = 10; j < 55 ; j += 5) { // collect 10 points of data 10 --> 50 cm
        char line[100] = "";
        cyBOT_Scan(90, &scan);
        snprintf(line, sizeof(line), "%.1f\t\t%d\t\t%d\n\r",scan.sound_dist, scan.IR_raw_val, j);

        int i;
        for(i = 0; line[i] != '\0'; i++) { // send the data from single sample
            cyBot_sendByte(line[i]);
        }
        timer_waitMillis(10000); // wait 10 seconds to be able to change distance
    }


    // wait until a button is pressed
    cyBOT_Scan(90, &scan);
    j = 10;
    while(j < 52) { // get data when the button is pressed
        if(button_num != 0) { // if the button was pressed
            char line[100] = "";
            cyBOT_Scan(90, &scan);

        snprintf(line, sizeof(line), "%.1f\t\t%d\t\t%d\n\r",scan.sound_dist, scan.IR_raw_val, j);
        int i;
        for(i = 0; line[i] != '\0'; i++) { // send the data from single sample
            cyBot_sendByte(line[i]);
        }
        j += 2;
        }
    }
*/

    // part 3
    /*
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0x00000007);
    //cyBOT_SERVO_cal();
    right_calibration_value = 243250;
    left_calibration_value = 1204000;
    cyBOT_Scan_t scan;
    cyBOT_Scan(90, &scan);

    while(1) {
        float IR_scaled;
        cyBOT_Scan(90, &scan);
        char line[100] = "";
        int z;
        float average;
        float accumulator = 0;
        float values[5];
        for (z = 0;z < 5; z++) {
            if (scan.IR_raw_val < 1460)
                {
                IR_scaled = 0.00006067*pow(scan.IR_raw_val, 2)-.21196*scan.IR_raw_val+209.041 - 1;
                }
            else {
                IR_scaled = (112.8276*pow(M_E, (-0.00108*scan.IR_raw_val))+4.831) - 1;
            }
            accumulator += IR_scaled;
            values[z] = IR_scaled;
        }
        float middle_values[3];
        int biggest = values[0];
        int smallest = values[0];

        for(z = 1; z < 5; z++) {
            if(values[z] > biggest) {
                biggest = values[z];
            }
            if(values[z] < smallest) {
                smallest = values[z];
            }
        }
        int j = 0;
        for(z = 0; z < 5; z++) {
            if(values[z] != biggest || values[z] != smallest) {
                middle_values[j] = values[z];
                j++;
            }
        }

      //  average = accumulator/5; // averaging mech (needed for lab)

        average = (middle_values[0] + middle_values[1] +  middle_values[2]) / 3;
        if(average < 10) { // average when between 9 and 12
            average += 2;
        }
        if(average > 37) {
            average -= 1;
        }
       lcd_clear();
       lcd_printf("%f\t%d",average,scan.IR_raw_val);
        snprintf(line, sizeof(line), "%.1f\t\t%d\t\t%.1f\n\r",scan.sound_dist, scan.IR_raw_val, IR_scaled);
        int i;
        for(i = 0; line[i] != '\0'; i++) { // send the data from single sample
            cyBot_sendByte(line[i]);
        }
        timer_waitMillis(3000);
    }

*/

    //part 4
    timer_init(); // Initialize Timer, needed before any LCD screen fucntions can be called
                      // and enables time functions (e.g. timer_waitMillis)
      lcd_init();   // Initialize the the LCD screen.  This also clears the screen.
      cyBot_uart_init();  // Initialize UART

      char my_data;       // Variable to get bytes from Client
      char command[100];  // Buffer to store command from Client
      int index = 0;      // Index position within the command buffer

      // Write to LCD so that we know the program is running
      lcd_printf("Running");
      oi_t *sensor_data = oi_alloc();
      oi_init(sensor_data);
      while(1)
      {
          char line[20] = "Hello\n";


             int j;
             for(j = 0; line[j] != '\0'; j++) { // send hello for first response
                 cyBot_sendByte(line[j]);
             }


         index = 0;  // Set index to the beginning of the command buffer
        my_data = cyBot_getByte_blocking();// Get first byte of the command from the Client

        // Get the rest of the command until a newline byte (i.e., '\n') received
        while(my_data != '\n' )
        {
            command[index] = my_data;
            index++;

            if(my_data == 'W') {
                move_forward(sensor_data, .05);
            }
            else if(my_data == 'A') {
                turn_left_degrees(sensor_data, 15);
            }
            else if(my_data == 'S') {
                move_back_distance(sensor_data, .05);
            }
            else if(my_data == 'D') {
                turn_right_degrees(sensor_data, 15);
            }
            else if(my_data == 'F') {
                oi_free(sensor_data);
                return 0;
            }
            else {
                oi_setWheels(0,0);


            }
            my_data = cyBot_getByte_blocking();
        }

        command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
        command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

        lcd_printf("Got: %s", command);  // Print received command to the LCD screen

        // Send a response to the Client (Starter Client expects the response to end with \n)
        // In this case I am just sending back the first byte of the command received and a '\n'
        cyBot_sendByte(command[0]);

        // Only send a '\n' if the first byte of the command is not a '\n',
        // to avoid sending back-to-back '\n' to the client
        if(command[0] != '\n')
        {
          cyBot_sendByte('\n');
        }

      }


	return 0;
	

}
