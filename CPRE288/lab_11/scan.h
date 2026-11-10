/*
 * scan.h
 *
 *  Created on: Oct 13, 2026
 *      Author: alex2168
 */

#ifndef SCAN_H_
#define SCAN_H_

extern int isFirstScan;
// New scan for only IR sensor
int scan(float *smallest_distance, char mode);
int scan_IR( int angle);


#endif /* SCAN_H_ */
