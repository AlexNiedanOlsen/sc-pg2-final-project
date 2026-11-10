/*
 * clean_data.c
 *
 *  Created on: Sep 23, 2026
 *      Author: alex2168
 */
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sensor-data.h"


static float THRESHOLD = .1;

void clean_data(float *sensor_data_array) {
    int i;

    for(i = 1; i < 90; i++) { // stops at 89 because no next next for i=90
        float last_point = sensor_data_array[i-1];
        float next_point = sensor_data_array[i+1];
        float next_next_point = sensor_data_array[i+2];
        if((fabs(sensor_data_array[i] - last_point) > THRESHOLD) && (fabs(sensor_data_array[i] - next_point) > THRESHOLD)) { // if one bad data point
            sensor_data_array[i] = last_point; // Didn't want to average last and next point
        } else if((fabs(sensor_data_array[i] - last_point) > THRESHOLD) && fabs(next_point - next_next_point) > THRESHOLD) { // if two bad data points in a row
            sensor_data_array[i] = last_point;
            next_point = next_next_point;
        } else{// do nothing
            sensor_data_array[i] = sensor_data_array[i];
        }




    }


}



