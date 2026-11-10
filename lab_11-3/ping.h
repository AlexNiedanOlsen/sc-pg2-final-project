/*
 * ping.h
 *
 *  Created on: Oct 27, 2026
 *      Author: kublk
 */

#ifndef PING_H_
#define PING_H_


extern volatile unsigned int rising_time;
extern volatile unsigned int falling_time;
extern volatile unsigned int edgeCount;

void ping_init();
int ping_read();
void TIMER3B_Handler();



#endif /* PING_H_ */
