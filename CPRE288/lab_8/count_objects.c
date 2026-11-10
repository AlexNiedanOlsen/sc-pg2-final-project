/*
 * count_objects.c
 *
 * Created on: Sep 23, 2026
 * Author: alex2168
 */

#include "Timer.h"
#include "lcd.h"
#include "scan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cyBot_Scan.h"
#include "uart_extra_help.h"

int get_num_objects(int data[], float *smallest_distance) {
    timer_init();
    lcd_init();
    uart_init(115200);

    int smallest_angle = 0;
    int angle = 0;
    int distance;
    float last_distance = -1;
    int current_obj_width = 0;
    int object_count = 0;
    int i = 0;
    int new_obj_flag = 0;
    float smallest = 10000;
    float distance_accumulator = 0;
    int average_values_incrementer = 0;
    int average_values[10];


    char heading[] = "Object#\t\tAngle\t\tDistance\t\tWidth\n\r";
    int z;
    for(z = 0; heading[z] != '\0'; z++) {
        uart_sendChar(heading[z]);
    }
    cyBOT_Scan_t scan;
    scan.IR_raw_val = 0;
    scan.sound_dist = 0;
    while(angle <= 180) {
        distance = data[i];

        if(distance < 60) { // When we see an object
            if(current_obj_width == 0) { // Nothing --> object
                current_obj_width = 2;
                last_distance = distance;
                distance_accumulator = distance;

            } else {
                if(fabs(last_distance - distance) < 15) { // Same object
                    current_obj_width += 2;
                    distance_accumulator += distance; // add to the accumlator every step
                } else { // object --> different object
                    new_obj_flag = 1;
                }
            }
        } else { // If we don't see an object
            if(current_obj_width > 0) { // object --> nothing
                new_obj_flag = 1;
            }
        }

        if(new_obj_flag) {
            object_count++;

            int average_distance = distance_accumulator / (current_obj_width / 2);
            distance_accumulator = 0;
            double radians = current_obj_width * (3.1415 / 180.0);
            float linear_width = 2 * average_distance * sin(radians / 2.0);

            if(linear_width < smallest) { // get the smallest width and position
                smallest = linear_width;
                smallest_angle = angle - (current_obj_width / 2);
                *smallest_distance = average_distance;
            }

            // if it's not the first scan and the object is to far to the right
            if(((angle - current_obj_width) == 0) && isFirstScan) {
                return; // Don't count the object
            }




                char line[50] = "";
            snprintf(line, sizeof(line), "%d\t\t%d\t\t%.1f\t\t\t%.1f\n\r",object_count, angle - (current_obj_width / 2), last_distance, linear_width);

            int j;
            for(j = 0; line[j] != '\0'; j++) {
                uart_sendChar(line[j]);
            }

            // get the middle distance of objects
            average_values[average_values_incrementer] = angle - (current_obj_width / 2);
            cyBOT_Scan(average_values[average_values_incrementer], &scan);
            cyBOT_Scan(average_values[average_values_incrementer], &scan);
            for(j = 0; j < 5; j++) { // Scan 5 times with ping
                timer_waitMillis(100);
                cyBOT_Scan(average_values[average_values_incrementer], &scan);
                distance_accumulator += scan.sound_dist;
            }
            char line1[20] = "";
            average_distance = distance_accumulator / 5; // get average distance
            snprintf(line1, sizeof(line1), "Ping distance: %d\n\r", average_distance);
            for(j = 0; line1[j] != '\0'; j++) {
                uart_sendChar_nonBlocking(line1[j]);
            }
            average_values_incrementer++;
            // Reset state
            if (distance < 60) {
                current_obj_width = 2;
                last_distance = distance;
            } else {
                current_obj_width = 0;
            }

            new_obj_flag = 0;
        }

        angle += 2;
        i++;
    }

    // if it goes past 180
    if(current_obj_width > 0) {
        if(isFirstScan) {
            object_count++;
            double radians = current_obj_width * (3.1415 / 180.0);


            average_values[average_values_incrementer] = angle - (current_obj_width / 2);
            int average_distance;
            int distance_accumulator_past;
            distance_accumulator_past = 0;
            cyBOT_Scan(average_values[average_values_incrementer], &scan);
            cyBOT_Scan(average_values[average_values_incrementer], &scan);
            cyBOT_Scan(average_values[average_values_incrementer], &scan);
            int j;
            for(j = 0; j < 5; j++) { // Scan 5 times with ping
                timer_waitMillis(100);
                cyBOT_Scan(average_values[average_values_incrementer], &scan);
                distance_accumulator_past += scan.sound_dist;
            }
            average_distance = distance_accumulator_past / 5; // get average distance


            float linear_width = 2 * last_distance * sin(radians / 2.0);
            if(linear_width < smallest) { // get the smallest width and position
                smallest = linear_width;
                smallest_angle = angle - (current_obj_width / 2);
                *smallest_distance = average_distance;
            }
            char line[50] = "" ;
            snprintf(line, sizeof(line), "%d\t\t%d\t\t%.1f\t\t\t%.1f\n\r" , object_count, angle - current_obj_width, last_distance, linear_width);
            for(j = 0; line[j] != '\0' ; j++) {
                uart_sendChar(line[j]);
            }

            char line1[20] = "" ;
            snprintf(line1, sizeof(line1), "Ping distance: %d\n\r" , average_distance);
            for(j = 0; line1[j] != '\0' ; j++) {
                uart_sendChar(line1[j]);
            }
        } else {
            // don't count object
        }
    }


    char line2[20] = "";
    snprintf(line2, sizeof(line2), "Smallest Width: %.1f\r\n", smallest);
    int j;
    for(j = 0; line2[j] != '\0'; j++) {
        uart_sendChar(line2[j]);
    }
    cyBOT_Scan(smallest_angle,  &scan);
    return smallest_angle;
}
