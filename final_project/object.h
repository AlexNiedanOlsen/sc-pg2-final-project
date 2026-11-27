#ifndef OBJECT_H
#define OBJECT_H

// Object data structure and  declarations for managing  memory and sending data over UART
typedef struct{
    float distance;
    unsigned char angle;
    float width;

} object_data;
// Function declarations for object memory management and data transmission
void reset_object_memory(void); 
void add_object_data(float distance, unsigned char angle, float width);
void send_object_data(void);

#endif
