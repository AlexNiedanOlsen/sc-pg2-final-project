/*
 * scan.h
 *
 *  Created on: Oct 13, 2026
 *      Author: alex2168
 */

#ifndef SCAN_H_
#define SCAN_H_
#include "cyBot_Scan.h"
extern int isFirstScan;
// New scan for only IR sensor
int scan(cyBOT_Scan_t scan, float *smallest_distance);
int scan_IR(cyBOT_Scan_t scan, int angle);


#endif /* SCAN_H_ */
