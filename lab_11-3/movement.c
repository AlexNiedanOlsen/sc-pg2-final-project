#include "open_interface.h"
#include <math.h>
#include "uart_extra_help.h"
/*
 * movement.c
 *
 *  Created on: Sep 1, 2026
 *      Author: kublk
 */
extern volatile  int tflag;

void move_forward (oi_t *sensor, double centimeters){
    double sum = 0;
     oi_setWheels(175, 175); // move forward; full speed
     while (sum < (centimeters * 9.85)) {
     oi_update(sensor);
     sum += sensor->distance;
     if(sensor->bumpLeft){
         left_bumper(sensor, (centimeters - (sum / 9.85)));
         break;
     }
     if(sensor->bumpRight){
         right_bumper(sensor, (centimeters - (sum / 9.85)));
         break;
     }
     if(tflag){
        tflag = 0;
        break;
     }
     }
     oi_setWheels(0, 0); // stop
}

void move_backward (oi_t *sensor, double centimeters){
    double sum = 0;
         oi_setWheels(-175, -175); // move forward; full speed
         while (sum > (centimeters * -9.85)) {
            oi_update(sensor);
            sum += sensor->distance;
            if(tflag){
                tflag = 0;
                break;
            }
         }
         oi_setWheels(0, 0); // stop
}

void turn_clockwise(oi_t *sensor, int degrees){
    double sum = 0;
         oi_setWheels(100, -100); // move forward; full speed
         while (sum < (degrees * 0.90)) {
         oi_update(sensor);
         sum += sensor->angle;
         if(tflag){
             tflag = 0;
             break;
         }
         }
         oi_setWheels(0, 0); // stop
}

void turn_counter_clockwise(oi_t *sensor, int degrees){
    double sum = 0;
         oi_setWheels(-100, 100); // move forward; full speed
         while (sum < (degrees * 0.85)) {
         oi_update(sensor);
         sum -= sensor->angle;
         if(tflag){
             tflag = 0;
             break;
         }
         }
         oi_setWheels(0, 0); // stop
}

void left_bumper(oi_t *sensor, double remaining){
    move_backward (sensor, 15);
    turn_counter_clockwise(sensor, 90);
    move_forward (sensor, 25);
    turn_clockwise(sensor, 100);
}

void right_bumper(oi_t *sensor, double remaining){
    move_backward (sensor, 15);
    turn_clockwise(sensor, 90);
    move_forward (sensor, 25);
    turn_counter_clockwise(sensor, 100);
}
