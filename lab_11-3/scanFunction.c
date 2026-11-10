/*
 * scanFunction.c
 *
 *  Created on: Nov 6, 2026
 *      Author: kublk
 */
#include "ping.h"
#include "adc.h"
#include "servo.h"
#include "Timer.h"

void scan180(int data[]){
    int i;
    servo_set(0);
    timer_waitMillis(10);
    for(i = 0; i < 180; i++){
        servo_set(i);
        timer_waitMillis(2);
        data[i] = adc_read();
    }
}
