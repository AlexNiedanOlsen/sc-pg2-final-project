

/**
 * main.c
 */

#include "scan.h"
#include "timer.h"
#include "uart_extra_help.h"
#include "cyBot_Scan.h"
#include "movement.h"
#include "open_interface.h"
#include "adc.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#define _PART1 0
#define _PART2 0
#define _PART3 0
#define _PART4 1


volatile  char uart_data;
volatile  char flag;
int isFirstScan = 1;


int main(void)
{
    adc_init();
    lcd_init();
    timer_init();
    uart_init(115200);
#if _PART1

    int value;
    adc_init();
    lcd_init();
    timer_init();
    lcd_clear();

    while(1) {
       value = adc_read();
       lcd_printf("IR: %d\n", value);

       timer_waitMillis(200);
       lcd_clear();
    }
#endif

#if _PART2
    int value;
    float IR_scaled;
    while(1) {

        float average;
        float accumulator = 0;
        float values[5];
        int z;
        for (z = 0;z < 5; z++) {
            value = adc_read();
            if (value < 1460)
            {
                IR_scaled = 0.00006067*pow(value, 2)-.21196*value+209.041 - 1;
            }
            else {
                IR_scaled = (112.8276*pow(M_E, (-0.00108*value))+4.831) - 1;
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


        lcd_printf("IR: %d \t\t Scaled: %f\n", value, average);
        timer_waitMillis(200);
        lcd_clear();
    }
#endif

#if _PART3
    int smallest_angle;
    float smallest_distance;
    smallest_distance = 1000000;
    uart_init(115200);
    cyBOT_init_Scan(0x3);
    cyBOT_Scan_t scan_data;
    scan_data.IR_raw_val = 0;
    scan_data.sound_dist = 0;
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);



    char byte = uart_receive_nonBlocking();
        while(1) {
            while(byte != 'T') { // manual mode
                    byte = uart_receive_nonBlocking();
                    if(byte != 0) {
                        if(byte == 'W') {
                            move_forward(sensor_data, .05);
                        }
                        else if(byte == 'A') {
                            turn_left_degrees(sensor_data, 15);
                        }
                        else if(byte == 'S') {
                            move_back_distance(sensor_data, .05);
                        }
                        else if(byte == 'D') {
                            turn_right_degrees(sensor_data, 15);
                        }
                        else if(byte == 'F') {
                            oi_free(sensor_data);
                        }
                        else if(byte == 'C') { // Scan
                            scan(scan_data, NULL);
                        }
                        else if(byte == 'I') { // IR sensor reading
                            cyBOT_Scan(90, &scan_data);
                            timer_waitMillis(200); // wait to get data
                            int z;
                            float average, IR_scaled;
                            float accumulator = 0;
                            float values[5];
                            for (z = 0;z < 5; z++) {
                                if (scan_data.IR_raw_val < 1460)
                                {
                                    IR_scaled = 0.00006067*pow(scan_data.IR_raw_val, 2)-.21196* scan_data.IR_raw_val +209.041 - 1;
                                }
                                else {
                                    IR_scaled = (112.8276*pow(M_E, (-0.00108*scan_data.IR_raw_val))+4.831) - 1;
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
                            average = (middle_values[0] + middle_values[1] +  middle_values[2]) / 3;
                            if(average < 10) { // average when between 9 and 12
                                average += 2;
                            }
                            if(average > 37) {
                                average -= 1;
                            }

                            char heading[] = "IR Value\r\n";

                            for(j = 0; heading[j] != '\0'; j++) {
                                uart_sendChar_nonBlocking(heading[j]);
                            }

                            char line[10] = "";

                            snprintf(line, sizeof(line), "%f", average);
                            for(j = 0; line[j] != '\0'; j++) {
                                uart_sendChar_nonBlocking(line[j]);
                            }

                            uart_sendChar_nonBlocking('\r');
                            uart_sendChar_nonBlocking('\n');

                        }
                    }
                        else {
                            oi_setWheels(0,0);
                        }
                    }

            while(byte != 'M') { // autonomous move
                // turn to object
                smallest_angle = scan(scan_data, &smallest_distance);
                isFirstScan = 0;

                if(smallest_distance < 10) {
                    return 0;
                }

                // turn to object
                if(smallest_angle < 90) {
                    turn_right_degrees(sensor_data, 90 - smallest_angle);
                } else if(smallest_angle > 90) {
                    turn_left_degrees(sensor_data, smallest_angle - 90);
                } else { // don't do anything

                }
                // move half way to object
                move_forward(sensor_data, ((smallest_distance / 100) / 3)); // convert to meters
                // scan

                timer_waitMillis(1000);
                byte =  uart_receive_nonBlocking();


            }

        }
        oi_free(sensor_data);
#endif
#if _PART4
    cyBOT_init_Scan(0x3);
    cyBOT_Scan_t scan_data;
    scan_data.IR_raw_val = 0;
    scan_data.sound_dist = 0;
    char my_data;       // Variable to get bytes from Client
    char command[100];  // Buffer to store command from Client
    int index = 0;      // Index position within the command buffer
    int smallest_angle;
    float smallest_distance;

    // Write to LCD so that we know the program is running
    lcd_printf("Running");
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    char byte = ' '; // I am not sure if I need to initilize this first

    while(byte != '\n') { // Wait for the client to send a string to confirm connection
        byte = uart_receive();
    }

        
    char line[20] = "Hello\n";
    int j;
    for(j = 0; line[j] != '\0'; j++) { // send hello for first response to let the client know that theres a connection
        uart_sendChar(line[j]);
    }


    char mode = 'M';

    while(1)
    {

        while(mode == 'M') {
        index = 0;  // Set index to the beginning of the command buffer
        my_data = uart_receive();// Get first byte of the command from the Client
        // Get the rest of the command until a newline byte (i.e., '\n') received
        while(my_data != '\n' )
        {
            command[index] = my_data;
            index++;
            if (my_data == 'M' || my_data == 'T') {
                mode = my_data;
                oi_setWheels(0, 0);
            }   else if(my_data == 'W') {
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
                else if(my_data == 'C') { // Scan
                    scan(scan_data, NULL, mode);
                    char end[] = "END\n"; // The current python code required this

                    int i;
                    for(i = 0; i < 4; i++) { // I have to send the null terminator
                        uart_sendChar(end[i]);
                    }
                }
                else {
                    oi_setWheels(0,0);

                }
                my_data = uart_receive();
            }

                command[index] = '\n';  // place newline into command in case one wants to echo the full command back to the Client
                command[index+1] = 0;   // End command C-string with a NULL byte so that functions like printf know when to stop printing

                lcd_printf("Got: %s", command);  // Print received command to the LCD screen

                

                // Only send a '\n' if the first byte of the command is not a '\n',
                // to avoid sending back-to-back '\n' to the client
                if(command[0] != '\n' && command[0] != 'C') // If the command is a scan there will be more
                {
                  uart_sendChar('\n');
                }
        }

        while(mode == 'T') {
            // turn to object
            smallest_angle = scan(scan_data, &smallest_distance, mode);
            isFirstScan = 0;

            if(smallest_distance < 10) {
                mode = 'M';
                oi_setWheels(0, 0);
                break;
            }

            // turn to object
            if(smallest_angle < 90) {
                turn_right_degrees(sensor_data, 90 - smallest_angle);
            } else if(smallest_angle > 90) {
                turn_left_degrees(sensor_data, smallest_angle - 90);
            } else { // don't do anything

            }
            // move half way to object
            move_forward(sensor_data, ((smallest_distance / 100) / 2)); // convert to meters
            // scan

            byte =  uart_receive_nonBlocking();
            if (byte == 'M') {       
                mode = 'M';
                oi_setWheels(0, 0);
                uart_sendChar('\n');
            }   else if (byte == 'T') { // If user presses autonomous mode button again the gui would get stucks
                    uart_sendChar('\n');
            }    

        }
              }
#endif
	return 0;
}
