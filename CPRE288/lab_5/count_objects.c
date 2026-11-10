/*
 * count_objects.c
 *
 * Created on: Sep 23, 2026
 * Author: alex2168
 */

#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cyBot_Scan.h"
#include "cyBot_uart.h"

int get_num_objects(float data[], int *smallest_position) {
    timer_init();
    lcd_init();
    cyBot_uart_init();

    int angle = 0;
    float distance;
    float last_distance = -1;
    int current_obj_width = 0;
    int object_count = 0;
    int i = 0;
    int new_obj_flag = 0;
    float smallest = 10000;
    float distance_accumulator = 0;


    char heading[] = "Object#\t\tAngle\t\tDistance\t\tWidth\n\r";
    int z;
    for(z = 0; heading[z] != '\0'; z++) {
        cyBot_sendByte(heading[z]);
    }

    while(angle <= 180) {
        distance = data[i];

        if(distance < .9) { // When we see an object
            if(current_obj_width == 0) { // Nothing --> object
                current_obj_width = 2;
                last_distance = distance;
                distance_accumulator = distance;

            } else {
                if(fabs(last_distance - distance) < 0.1) { // Same object
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

            float average_distance = distance_accumulator / (current_obj_width / 2);
            distance_accumulator = 0;
            double radians = current_obj_width * (3.1415 / 180.0);
            float linear_width = 2 * average_distance * sin(radians / 2.0);

            if(linear_width < smallest) { // get the smallest width and position
                smallest = linear_width;
                *smallest_position = angle - (current_obj_width / 2);
            }

            char line[100] = "";
            snprintf(line, sizeof(line), "%d\t\t%d\t\t%.1f\t\t\t%.1f\n\r",object_count, angle - current_obj_width, last_distance, linear_width);

            int j;
            for(j = 0; line[j] != '\0'; j++) {
                cyBot_sendByte(line[j]);
            }

            // Reset state
            if (distance < .9) {
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
        object_count++;
        float average_distance = distance_accumulator / (current_obj_width / 2);

        double radians = current_obj_width * (3.1415 / 180.0);
        float linear_width = 2 * last_distance * sin(radians / 2.0);

        if(linear_width < smallest) { // get the smallest width and position
            smallest = linear_width;
            *smallest_position = angle - (current_obj_width / 2);
        }

        char line[100] = "";
        snprintf(line, sizeof(line), "%d\t\t%d\t\t%.1f\t\t\t%.1f\n\r", object_count, angle - current_obj_width, last_distance, linear_width);

        int j;
        for(j = 0; line[j] != '\0'; j++) {
            cyBot_sendByte(line[j]);
        }
    }


    char line[50] = "";
    snprintf(line, sizeof(line), "Smallest Width: %.1f\r\n", smallest);
    int j;
    for(j = 0; line[j] != '\0'; j++) {
        cyBot_sendByte(line[j]);
    }

    return object_count;
}
