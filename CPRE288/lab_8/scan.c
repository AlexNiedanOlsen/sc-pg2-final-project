/*
 * scan.c
 *
 *  Created on: Oct 13, 2026
 *      Author: alex2168
 */

#include "scan.h"
#include "timer.h"
#include "uart_extra_help.h"
#include "cyBot_Scan.h"
#include "clean_data.h"
#include "count_objects.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

// Used in clean data function
int sensor_data_array[] = {
    150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
    160, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170,
    190, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60
};





int scan(cyBOT_Scan_t scan, float *smallest_distance) {
    int i, j, z;
    right_calibration_value = 290500;
    left_calibration_value = 1282750;
    int smallest_angle;

    FILE *fptr;
    fptr = fopen("output.txt", "a");
    if (fptr == NULL) {
        printf("Error: Could not open or create the file.\n");
        return;
    }

    char heading[] = "Angle(Degrees)    Distance(m)\r\n";



    for(j = 0; heading[j] != '\0'; j++) {
        uart_sendChar(heading[j]);
    }

    for (i = 0; i < 5; i++) {
        cyBOT_Scan(0, &scan);
    }

    for (i = 0; i <= 180; i+= 2) {
        timer_waitMillis(10);
        int distance = scan_IR(scan, i);
        sensor_data_array[i/2] = distance;
        char line[20] = "";
        snprintf(line, sizeof(line), "%d\t\t\t%d\r\n", i, distance);
        fprintf(fptr, "%d\t\t\t%d\r\n", i, distance);
        for(j = 0; line[j] != '\0'; j++) {
            uart_sendChar(line[j]);
        }
    }



    clean_data(sensor_data_array);


    for(i = 0; i <= 180; i += 2) {
        char line[20] = "";
        snprintf(line, sizeof(line), "%d\t\t\t%d\r\n", i, sensor_data_array[i/2]);
        for(j = 0; line[j] != '\0'; j++) {
            uart_sendChar_nonBlocking(line[j]);
        }
    }


    smallest_angle = get_num_objects(sensor_data_array, smallest_distance);





    return smallest_angle;

    fclose(fptr);


}

int scan_IR(cyBOT_Scan_t scan, int angle) { // Returns the average distance of 5 scans
    int z;
    float IR_scaled;
    float accumulator = 0;
    float values[5];



    for (z = 0;z < 5; z++) {
        cyBOT_Scan(angle, &scan);
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

    return (int)accumulator / 5;




}
