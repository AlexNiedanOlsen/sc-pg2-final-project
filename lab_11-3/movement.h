/*
 * movement.h
 *
 *  Created on: Sep 1, 2026
 *      Author: kublk
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include <math.h>

extern volatile  int tflag; 

void move_forward(oi_t *sensor, double centimeters);
void move_backward(oi_t *sensor, double centimeters);
void turn_clockwise(oi_t *sensor, int degrees);
void turn_counter_clockwise(oi_t *sensor, int degrees);
void left_bumper(oi_t *sensor, double remaining);
void right_bumper(oi_t *sensor, double remaining);



#endif /* MOVEMENT_H_ */
