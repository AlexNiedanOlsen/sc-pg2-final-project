/*
 * scaninfo.c
 *
 *  Created on: Oct 13, 2026
 *      Author: kublk
 */

#include "open_interface.h"
#include <math.h>
#include "cyBot_Scan.h"
#include "uart_extra_help.h"
#include <string.h>
#include "movement.h"
#include "scaninfo.h"
#include <stdlib.h>

void fillScanArray(int scanIR[]){
    int i;
    int j;
    int rawdata = 0;
    cyBOT_Scan_t scan;
    for (i = 0; i < 91; i++){
        rawdata = 0;
        for (j = 0; j < 2; j++){
            cyBOT_Scan((i * 2), &scan);
            rawdata += scan.IR_raw_val;
        }
        if(tflag){
            tflag = 0;
            break;
        }
        rawdata = rawdata / 2;
        scanIR[i] = (int) (10772064.9 / pow((rawdata / 1.0), 1.77316));    
    }
}

void printData(int pureData[91]){
    int i, j;
    int len;
    char printer[20];
    uart_sendChar('\r');
    uart_sendChar('\n');
    for (i = 0; i < 91; ++i){
        len = sprintf(printer, "%d   %d", i * 2, pureData[i]);

        for (j = 0; j < len; ++j){
            uart_sendChar(printer[j]);
        }
        uart_sendChar('\r');
        uart_sendChar('\n');
    }
}

void filterData(int data[91]){
    int i;
    for(i = 0; i < 91; i++){
        if(data[i] > 120){
            data[i] = 120;
        }
    }
    for(i = 2; i < 88; i++){
        if((abs(data[i] - data[i - 1]) > 20) && 
        (abs(data[i] - data[i + 1]) > 20)){
            data[i] = (data[i + 1] + data[i - 1]) / 2;
        }
        if((abs(data[i] - data[i - 2]) > 20) && 
        (abs(data[i] - data[i + 2]) > 20)){
            data[i] = (data[i + 2] + data[i - 2]) / 2;
        }
    }
    if(abs(data[3] - data[0]) > 10){
        for(i = 0; i < 3; i++){
            data[i] = data[3];
        }
    }
    if(abs(data[90] - data[87]) > 10){
        for(i = 0; i < 3; i++){
            data[i + 88] = data[87];
        }
    }
}


int identifyObjects(object objects[8], int data[91]){
    memset(objects, 0, 8 * sizeof(objects[0]));
    int rising = 0;
    int i;
    int objCount = 0;
    int objectStart = 0, objectEnd = 0;
    for(i = 0; i < 89; ++i){
        //if there is a rise in data by 25 or 
        //a smaller rise of 13 and data is already low
        if((data[i] > data[i + 2] + 25) || 
        ((data[i] > data[i + 2] + 13) && (data[i] < 55))){
            rising = 1;
            objectStart = i * 2;
        }
        else if((rising) && ((data[i] + 25 < data[i + 2]) ||
        ((i == 88) && (objectStart < 173)) ||
        ((data[i] + 13 < data[i + 2]) && (data[i] < 55)))){
            rising = 0;
            objectEnd = i * 2;
            objects[objCount].distance  = data[i - 1];
            objects[objCount].angle     = objectStart + (objectEnd - objectStart) / 2;
            objects[objCount].width     = objectEnd - objectStart;
            objects[objCount].trueWidth = 2 * data[i - 1] * 
                tan(3.14159 * (objectEnd - objectStart) / 360.0);
            ++objCount;
            if(objectEnd > 177){
                objects[objCount].width     = objectEnd - objectStart + 14;
                            objects[objCount].trueWidth = 2 * data[i - 1] *
                                tan(3.14159 * (objectEnd - objectStart + 14) / 360.0);
            }
        }
    }
    return objCount;
}

//change to a scan of 5 angles +1 into array
void pingObjectDistance(object objects[8], int objCount){
    cyBOT_Scan_t scan;
    int i, j;
    float pingData[5];
    for(i = 0; i < objCount; i++){
        cyBOT_Scan(objects[i].angle, &scan);
        timer_waitMillis(100);
        for(j = 0; j < 5; j++){
            cyBOT_Scan(objects[i].angle + j - 2, &scan);
            pingData[j] = scan.sound_dist;
        }
        int index = 0;
        for(j = 0; j < 4; j++){
            if(pingData[j + 1] < pingData[index]){
                index = j + 1;
            }
        }
        objects[i].distance = (int) pingData[index];
        objects[i].trueWidth = 2 * objects[i].distance *
            tan(3.14159 * objects[i].width / 360.0);
    }
}


void printObjects(object objects[8], int objCount){
    int i, j;
    char objMsg[80];
    char msg[] = "Object#   Angle          Distance    Width";
    for(i = 0; i < strlen(msg); i++){
        uart_sendChar(msg[i]);
    }
    uart_sendChar('\n');
    uart_sendChar('\r');
    for(i = 0; i < objCount; i++){
        sprintf(objMsg, "%d            %d                 %d            %.2f",
        i + 1, objects[i].angle, objects[i].distance, objects[i].trueWidth);
        for(j = 0; j < strlen(objMsg); j++){
            uart_sendChar(objMsg[j]);
        }
        uart_sendChar('\n');
        uart_sendChar('\r');
    }
}


int identifySmallest(object objects[8], int objCount){
    int i;
    int smallest = 0;
    for(i = 1; i < objCount; i++){
        if(objects[i].trueWidth < objects[smallest].trueWidth){
            smallest = i;
        }
    }
    return smallest;

}


void faceSmallest(object objects[8], int index, oi_t *sensor){
    int angleChange;
    angleChange = 90 - objects[index].angle;
    if(angleChange < 0){
        turn_clockwise(sensor, abs(angleChange));
    }
    else{
        turn_counter_clockwise(sensor, angleChange);
    }
}


void getCloserToTarget(oi_t *sensor, object objects[8], int index){
    move_forward(sensor, objects[index].distance * 0.5);
}


void getNextToObject(oi_t *sensor, object objects[8], int index){
    move_forward(sensor, objects[index].distance - 10);
}

void stateNextMove(object objects[8], int index, int state){
    char txt[60];
    int i;
    if(state == 0){
        sprintf(txt, "Turn %d and move %d", 90 - objects[index].angle, objects[index].distance);
    }
    else if(state == 1){
        sprintf(txt, "Turn 90 left and move 10 (object to right)");
    }
    else{
        sprintf(txt, "Turn 90 right and move 10 (object to right)");
    }
    for(i = 0; i < strlen(txt); i++){
        uart_sendChar(txt[i]);
    }
    uart_sendChar('\r');
    uart_sendChar('\n');
}

int toggleMode(int manual){
    char txt[20];
    int i;
    if(manual){
        manual = 0;
        sprintf(txt, "Automatic On");
        for(i = 0; i < strlen(txt); i++){
            uart_sendChar(txt[i]);
        }
    }
    else{
        manual = 1;
        sprintf(txt, "Manual On");
        for(i = 0; i < strlen(txt); i++){
            uart_sendChar(txt[i]);
        }
    }
    uart_sendChar('\r');
    uart_sendChar('\n');
    return manual;
}

int identifyObjectsInWay(object objects[8], int index, int objCount){
    int i;
    for(i = 0; i < objCount; i++){
        if(objects[i].distance < 10 && !(index == i)){
            if(objects[i].angle < 90){
                return 1;
            }
            else{
                return 2;
            }
        }
    }
    return 0;
}

void moveAroundObject(oi_t *sensor, int state){
    if(state == 1){
        turn_counter_clockwise(sensor, 90);
        move_forward(sensor, 10);
        turn_clockwise(sensor, 90);
    }
    else{
        turn_clockwise(sensor, 90);
        move_forward(sensor, 10);
        turn_counter_clockwise(sensor, 90);
    }
}
