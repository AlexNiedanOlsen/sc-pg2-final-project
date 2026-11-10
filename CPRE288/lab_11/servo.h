/*
 * servo.h
 *
 *  Created on: Nov 3, 2026
 *      Author: alex2168
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "button.h"
#include "timer.h"
#include "lcd.h"

// Initialize the servo
// Timer 1B
// Pin 5B
void servo_init();

// Change the match values
void servo_adjust_match();


// turn to the position specified by the user
void turn_to(int degree);



#endif /* SERVO_H_ */
