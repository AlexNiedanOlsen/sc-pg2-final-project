#include "object.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_OBJECTS 15

static object_data object_memory[MAX_OBJECTS];
static int object_count;


void reset_object_memory(void) {
    // Resets object memory and count
    object_count = 0;
}

void add_object_data(float distance, unsigned char angle, float width){
    // Adds object data if there is space
    if(object_count < MAX_OBJECTS){
        object_memory[object_count].distance =distance;
        object_memory[object_count].angle = angle;
        object_memory[object_count].width = width;
        object_count++;
    }
}
void send_object_data(void){
    // Sends object data with uart
    unsigned char buffer[1 +(MAX_OBJECTS * 12)];

    // Store the number of objects
    buffer[0] = object_count;

    int index = 1;

    for(int i = 0; i < object_count; i++){

        // Copies distance into buffer (4 bytes)
        memcpy(&buffer[index], &object_memory[i].distance, 4);
        index += 4;

        // Copies angle into buffer (4 bytes)
        uint32_t angle = object_memory[i].angle;
        memcpy(&buffer[index], &angle, 4);
        index += 4;

        // Copies width into buffer (4 bytes)
        memcpy(&buffer[index],&object_memory[i].width, 4);
        index += 4;
    }
    // Send data over uart
    uart_send_bytes(buffer, index);
}
