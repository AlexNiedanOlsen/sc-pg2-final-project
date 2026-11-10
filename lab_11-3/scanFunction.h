/*
 * scanFunction.h
 *
 *  Created on: Nov 6, 2026
 *      Author: kublk
 */

#ifndef SCANFUNCTION_H_
#define SCANFUNCTION_H_

typedef struct {
    int angle;
    int distance;
    int width;
    double trueWidth;
} object;

void scan180(int data[]);

#endif /* SCANFUNCTION_H_ */
