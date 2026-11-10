/*
 * movement.h
 *
 *  Created on: Sep 1, 2026
 *
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"

void move_forward(oi_t *sensor_data, float distance);
void move_back_distance(oi_t *sensor_data, float distance);
void turn_left_degrees(oi_t *sensor_data, int degrees);
void turn_right_degrees(oi_t *sensor_data, int degrees);
bool detect_collision(oi_t *sensor_data);



#endif /* MOVEMENT_H_ */
