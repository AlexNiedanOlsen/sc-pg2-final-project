
#include "clean_data.h"
#include "count_objects.h"
#include <stdio.h>
#include <stdint.h>
//  gcc main.c count_objects.c clean_data.c -o program
float sensor_data_array[91] = { 
3.1, 
0.4, 
0.4, 
3.1, 
3.1, 
3.1, 
2.4, 
2.4, 
2.3, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
3.1, 
2.0, 
1.9, 
1.9, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
0.4, 
3.0, 
3.0, 
3.0, 
3.0, 
3.0, 
3.0, 
3.0, 
3.0
}; 

int main(void) {
    clean_data(sensor_data_array);
    FILE *file = fopen("sensor_output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file, "Angle(Degrees)    Distance(m)\r\n");
    for(int i = 0; i < 180; i+=2) {
         fprintf(file, "%-26d%.1f\r\n", i, sensor_data_array[i/2]);
    }
    int smallest_position = 0;
    int num_objects = get_num_objects(sensor_data_array, &smallest_position);
    printf("Num objects: %d\n", num_objects);
    fclose(file);
    return 0;
}