

/**
 * main.c
 */
#include "scan.h"
#include "timer.h"
#include "uart_extra_help.h"
#include "cyBot_Scan.h"
#include "movement.h"
#include "open_interface.h"
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
{ // Go to lab 6 template to calibrate

    int smallest_angle;
    float smallest_distance;
    smallest_distance = 1000000;
    uart_init(115200);
    cyBOT_init_Scan(0x7);
    cyBOT_Scan_t scan_data;
    scan_data.IR_raw_val = 0;
    scan_data.sound_dist = 0;
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);


#if _PART1
    smallest_angle = scan(scan_data, &smallest_distance);
    cyBOT_Scan(smallest_angle, &scan_data);
#endif

#if _PART2
    // must get smallest distance and angle
    smallest_angle = scan(scan_data, &smallest_distance);
    isFirstScan = 0;


    // autonomous mode
    while(smallest_distance > 10) {
        // turn to object
        if(smallest_angle < 90) {
            turn_right_degrees(sensor_data, 90 - smallest_angle);
        } else if(smallest_angle > 90) {
            turn_left_degrees(sensor_data, smallest_angle - 90);
        } else { // don't do anything

        }
        // move half way to object
        move_forward(sensor_data, ((smallest_distance / 100) / 2)); // convert to meters
        timer_waitMillis(100);
        // scan
        smallest_angle = scan(scan_data, &smallest_distance);
    }
#endif

#if _PART3
    uart_init(115200);
    cyBOT_init_Scan(0x7);

    while(1) {
        char byte = uart_receive_nonBlocking();
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
                int  i;
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
#endif
#if _PART4
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
                return;
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

            timer_waitMillis(1000);
            byte =  uart_receive_nonBlocking();


        }

    }
#endif

	return 0;
}
