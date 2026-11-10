/*
 * scaninfo.h
 *
 *  Created on: Oct 13, 2026
 *      Author: kublk
 */

#ifndef SCANINFO_H_
#define SCANINFO_H_

typedef struct {
    int angle;
    int distance;
    int width;
    double trueWidth;
} object;

void fillScanArray(int scanIR[]);
void printData(int pureData[]);
void filterData(int data[]);
int identifyObjects(object objects[], int data[]);
void pingObjectDistance(object objects[], int objCount);
void printObjects(object objects[], int objCount);
int identifySmallest(object objects[], int objCount);
void faceSmallest(object objects[], int index, oi_t *sensor);
void getCloserToTarget(oi_t *sensor, object objects[], int index);
void getNextToObject(oi_t *sensor, object objects[], int index);
void stateNextMove(object objects[], int index, int state);
int toggleMode(int manual);
int identifyObjectsInWay(object objects[], int index, int objCount);
void moveAroundObject(oi_t *sensor, int state);




#endif /* SCANINFO_H_ */
