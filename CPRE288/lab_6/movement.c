/*
 * movement.c
 *
 *  Created on: Sep 1, 2026
 *
 *
 *
 *
 */

#include <movement.h>
#include "open_interface.h" // Fixed include syntax
#include <stdlib.h>         // Added for abs()

// Move forward a set distance in meters
void move_forward(oi_t *sensor_data, float distance){
    int mmdistance = distance * 1000;
    double sum = 0;

    oi_setWheels(100, 100); // Move forward at full speed

    while (sum < mmdistance) {
        int collision = detect_collision(sensor_data);
        if(collision != 0) {
            sum -= 70;
        }
            oi_setWheels(100, 100); // Resume forward speed

        oi_update(sensor_data);
        sum += sensor_data->distance;
    }
    oi_setWheels(0, 0); // Stop
}

// Move backward a set distance in meters
void move_back_distance(oi_t *sensor_data, float distance){
    int mmdistance = abs(distance * 1000); // Target is positive
    double sum = 0;                   // Tracks absolute distance moved
    int speed = -250;
    if(distance < 0) {
        speed = 250;
    }
    oi_setWheels(speed, speed); // Move backward

    while (sum < mmdistance) {
        oi_update(sensor_data);
        sum += abs(sensor_data->distance); // Use absolute value since backward distance is negative
    }
    oi_setWheels(0, 0); // Stop
}

// Turn left using a timed delay
void turn_left_degrees(oi_t *sensor_data, int degrees) {
    int time_factor = 15;
    int degrees_scaled = degrees * time_factor;

    oi_setWheels(121, -121);
    timer_waitMillis(degrees_scaled);
    oi_setWheels(0, 0);
}

// Turn right using a timed delay
void turn_right_degrees(oi_t *sensor_data, int degrees) {
    int time_factor = 15;
    int degrees_scaled = degrees * time_factor;

    oi_setWheels(-121, 121);
    timer_waitMillis(degrees_scaled);
    oi_setWheels(0, 0);
}

// Check bumpers and execute evasive maneuvers
bool detect_collision(oi_t *sensor_data) {
    oi_update(sensor_data); // Get current state of all sensors

    // Case 1: Both bumpers hit simultaneously
    if (sensor_data->bumpLeft && sensor_data->bumpRight) {
        move_back_distance(sensor_data, 0.15);
        turn_right_degrees(sensor_data, 90); // Default to right escape on center hit
        move_back_distance(sensor_data, -0.25);
        turn_left_degrees(sensor_data, 90);
        return true;
    }

    // Case 2: Only Left bumper hit
    if (sensor_data->bumpLeft) {
        move_back_distance(sensor_data, 0.15);
        turn_right_degrees(sensor_data, 90);
        move_back_distance(sensor_data, -0.25);
        turn_left_degrees(sensor_data, 90);
        return true;
    }

    // Case 3: Only Right bumper hit
    if (sensor_data->bumpRight) {
        move_back_distance(sensor_data, 0.15);
        turn_left_degrees(sensor_data, 90);
        move_back_distance(sensor_data, -0.25);
        turn_right_degrees(sensor_data, 90);
        return true;
    }

    return 0;
}





