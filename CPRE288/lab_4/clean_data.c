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
static float THRESHOLD = .5;

void clean_data(float *sensor_data_array) {

    uint8_t i = 1;
    float last_point = sensor_data_array[i-1];
    float next_point = sensor_data_array[i+1];

    for(i = 1; i < 91; i++) {
        if((abs(sensor_data_array[i] - last_point) > THRESHOLD) && (abs(sensor_data_array[i] - next_point) > THRESHOLD)) {
            sensor_data_array[i] = last_point; // Didn't want to average last and next point
        }
    }


}


