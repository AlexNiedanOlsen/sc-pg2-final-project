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
#include "lcd.h"


/*
 * Used for debugging differences in the motor encoders
 * + = left ahead
 * - = right ahead
 */

int ramp_up_difference = 0;
int ramp_down_difference = 0;
int steady_difference = 0;




// Move forward a set distance in meters
void move_forward(oi_t *sensor_data, float distance){
    int mmdistance = distance * 500;
    double sum = 0;
    lcd_init();
    lcd_clear();


    // No motor encoders
    /*
    oi_setWheels(50, 50); // Move forward at full speed

    while (sum < mmdistance) {
        oi_setWheels(100, 100); // Resume forward speed


        oi_update(sensor_data);
        sum += sensor_data->distance;
    }
    */



    // Set the motor encoder count to 0
    // Do the distance based on the encoder count
    // according to datasheet that are 508.8 encoder ticks per revolution
    // 226.2 mm per revolution

    float total_num_ticks = (mmdistance / 226.2) * 508.8;



    // Multiplying mm distance by 2 gets correct value
    // we want to ramp up for 1/4 of the distance and ramp down for 1/4
    // so not multiplying by 2 will get us what we want

    ramp_up(sensor_data, distance / 2, 90);

    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = 0;
    int sum_right = 0;
    float left_diff = 0;
    float right_diff = 0;


    // used to sum up all of the error
    int num_cycles = 1;
    float accumulated_error = 0;
    float average_error = 0;
    float error_diff = 0;
    float last_error = 0;

    while ((sum_left < total_num_ticks) && (sum_right < total_num_ticks)) {

        oi_update(sensor_data);

        left_diff = sensor_data->leftEncoderCount - last_left;
        right_diff = sensor_data->rightEncoderCount - last_right;

        // Total distance
        sum_left += left_diff;
        sum_right += right_diff;

        float error = left_diff - right_diff;
        accumulated_error += (left_diff - right_diff);
        // if the average error is - then right is faster
        // if the average error is + then left is faster
        average_error = accumulated_error / num_cycles;

        error_diff = error - last_error;





        // constants that terms are multipled by
       float Kp = 5;
       float Ki = .95; // don't go above 1.1 otherwise there is integral winding
       float Kd = .45;
       float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

       lcd_printf("Steady Speed\nLeft: %d \nRight: %d", sum_left, sum_right);


       int left_speed = 90 - (int)multiply_factor;
       int right_speed = 90 + (int)multiply_factor;



       // Make sure the speed doesn't go above 100 or below 0
       if(left_speed > 100) {
           left_speed = 100;
       }
       if(left_speed < 0) {
           left_speed = 0;
       }
       if(right_speed > 100) {
           right_speed = 100;
       }
       if(right_speed < 0) {
           right_speed = 0;
       }

       oi_setWheels(right_speed, left_speed);

       last_error = error;

       last_left = sensor_data->leftEncoderCount;
       last_right = sensor_data->rightEncoderCount;

       num_cycles += 1;


    }
    steady_difference = sum_left - sum_right;


    ramp_down(sensor_data, distance / 2, 100);

    lcd_clear();
    lcd_printf("up error: %d\ndown error: %d\nsteady error: %d", ramp_up_difference, ramp_down_difference, steady_difference);
    //lcd_printf("Error: %d", steady_difference);
}

// Move backward a set distance in meters
// Same as move forward but negative
void move_back_distance(oi_t *sensor_data, float distance){
    int mmdistance = distance * 500 * 2;


    float total_num_ticks = (mmdistance / 226.2) * 508.8;

    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = total_num_ticks;
    int sum_right = total_num_ticks;
    float left_diff = 0;
    float right_diff = 0;



    oi_setWheels(-100, -100);
    while ((sum_left > 0) && (sum_right > 0)) {

        oi_update(sensor_data);

        left_diff = last_left - sensor_data->leftEncoderCount;
        right_diff = last_right - sensor_data->rightEncoderCount;

        sum_left -= left_diff;
        sum_right -= right_diff;

        // If the left motor encoder count is greater than the right encoder count
        if(left_diff > right_diff) {
            float multiply_factor = (left_diff / right_diff); // When just right/left it still goes to the right a little
            oi_setWheels(-100 * multiply_factor, -100);


        } else if (right_diff > left_diff) {
             // When just right/left it still goes to the righ a little
            float multiply_factor = (right_diff / left_diff);
            oi_setWheels(-100, -100 * multiply_factor);

        } else { // If they are the same value
            oi_setWheels(-100, -100);
        }


        last_left = sensor_data->leftEncoderCount;
        last_right = sensor_data->rightEncoderCount;


    }

    oi_setWheels(0, 0); // Stop
}


// Circumference of robot = 738.274273594mm
// Turn left using a timed delay
#define ROBOT_CIRCUMFERANCE 738.274273594
void turn_left_degrees(oi_t *sensor_data, int degrees) {
    // Total distance travled is (degrees/360) * Wheel_CIRCUMFERANCE
    lcd_init();
    lcd_clear();
    float mmdistance = (degrees/360.0) * ROBOT_CIRCUMFERANCE;

    float total_num_ticks = (mmdistance / 226.2) * 508.8 * .99;

    oi_update(sensor_data);
    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = 0;
    int sum_right = 0;
    float left_diff = 0;
    float right_diff = 0;

    // used to sum up all of the error
    int num_cycles = 1;
    float accumulated_error = 0;
    float error_diff = 0;
    float last_error = 0;

    int left_speed;
    int right_speed;

    // Only want to go 8/10 of the circumferance then want to ramp down
    while (((sum_left + sum_right) / 2.0) < (total_num_ticks * .8)) {
        oi_update(sensor_data);

        left_diff = sensor_data->leftEncoderCount - last_left;
        right_diff = sensor_data->rightEncoderCount - last_right;

        last_left = sensor_data->leftEncoderCount;
        last_right = sensor_data->rightEncoderCount;

        // Total distance
        sum_left += abs(left_diff);
        sum_right += abs(right_diff);

        float error = sum_left - sum_right;
        accumulated_error += error;
        // if the average error is - then right is faster
        // if the average error is + then left is faster


        error_diff = error - last_error;

        // constants that terms are multipled by
        float Kp = 1;
        float Ki = .1; // don't go above 1.1 otherwise there is integral winding
        float Kd = .6;
        float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

        lcd_printf("Steady Speed\nLeft: %d \nRight: %d", sum_left, sum_right);


        left_speed = -30 + (int)multiply_factor;
        right_speed = 30 + (int)multiply_factor;

        // Make sure the speed doesn't go above 100 or below 0
        if(left_speed < -70) {
            left_speed = -70;
        }
        if(left_speed > 0) {
            left_speed = -10;
        }
        if(right_speed > 70) {
            right_speed = 70;
        }
        if(right_speed < 0) {
            right_speed = 10;
        }

        oi_setWheels(right_speed, left_speed);

        last_error = error;

        last_left = sensor_data->leftEncoderCount;
        last_right = sensor_data->rightEncoderCount;

        num_cycles += 1;


    }
    ramp_down_left(sensor_data, degrees * .2, 30);


    oi_setWheels(0, 0);
}

// Turn right using a timed delay
void turn_right_degrees(oi_t *sensor_data, int degrees) {
    // Total distance travled is (degrees/360) * Wheel_CIRCUMFERANCE
        lcd_init();
        lcd_clear();
        float mmdistance = (degrees/360.0) * ROBOT_CIRCUMFERANCE;

        float total_num_ticks = (mmdistance / 226.2) * 508.8;

        oi_update(sensor_data);
        int last_left = sensor_data->leftEncoderCount;
        int last_right = sensor_data->rightEncoderCount;
        int sum_left = 0;
        int sum_right = 0;
        float left_diff = 0;
        float right_diff = 0;

        // used to sum up all of the error
        int num_cycles = 1;
        float accumulated_error = 0;
        float error_diff = 0;
        float last_error = 0;

        int left_speed;
        int right_speed;

        // Only want to go 9/10 of the circumferance then want to ramp down
        while (((sum_left + sum_right) / 2.0) < total_num_ticks) {
            oi_update(sensor_data);

            left_diff = sensor_data->leftEncoderCount - last_left;
            right_diff = sensor_data->rightEncoderCount - last_right;

            last_left = sensor_data->leftEncoderCount;
            last_right = sensor_data->rightEncoderCount;

            // Total distance
            sum_left += abs(left_diff);
            sum_right += abs(right_diff);

            float error = sum_left - sum_right;
            accumulated_error += error;
            // if the average error is - then right is faster
            // if the average error is + then left is faster


            error_diff = error - last_error;

            // constants that terms are multipled by
            float Kp = 1;
            float Ki = .1; // don't go above 1.1 otherwise there is integral winding
            float Kd = .6;
            float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

            lcd_printf("Steady Speed\nLeft: %d \nRight: %d", sum_left, sum_right);


            left_speed = 30 - (int)multiply_factor;
            right_speed = -30 - (int)multiply_factor;

            // Make sure the speed doesn't go above 100 or below 0
            if(left_speed > 70) {
                left_speed = 70;
            }
            if(left_speed < 0) {
                left_speed = 10;
            }
            if(right_speed < -70) {
                right_speed = -70;
            }
            if(right_speed > 0) {
                right_speed = -10;
            }

            oi_setWheels(right_speed, left_speed);

            last_error = error;

            last_left = sensor_data->leftEncoderCount;
            last_right = sensor_data->rightEncoderCount;

            num_cycles += 1;


        }



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


// Given a speed and a distance(in meters), increase the speed to that distance at a ceratin rate
void ramp_up(oi_t *sensor_data, float distance, int speed){
    int mmdistance = distance * 500; // Off by a factor of 2 so had to multiply it by 2


    float distance_interval, speed_interval; // Every distance interval increase speed by a certain amount

    distance_interval = mmdistance / 6; // Have 6 points in the ramp up function where you increase speed
    speed_interval = speed / 6;

    float total_num_ticks = (mmdistance / 226.2) * 508.8;

    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = 0;
    int sum_right = 0;
    float left_diff = 0;
    float right_diff = 0;


    // used to sum up all of the error
    int num_cycles = 1;
    float accumulated_error = 0;
    float average_error = 0;
    float error_diff = 0;
    float last_error = 0;



    accumulated_error += (left_diff - right_diff);

    // if the average error is - then right is faster
    // if the average error is + then left is faster
    average_error = accumulated_error / num_cycles;

    // ramp up the speed 6 times
    int i;
    float current_speed = 0; // Speed that will be used to track current speed
    for(i = 6; i > 0; i--) {
        current_speed += speed_interval;
        oi_setWheels(current_speed, current_speed);

        while((sum_left < total_num_ticks / i) && (sum_right < total_num_ticks / i)) {


            oi_update(sensor_data);

            left_diff = sensor_data->leftEncoderCount - last_left;
            right_diff = sensor_data->rightEncoderCount - last_right;

            // Total distance
            sum_left += left_diff;
            sum_right += right_diff;

            float error = left_diff - right_diff;
            accumulated_error += (left_diff - right_diff);
            // if the average error is - then right is faster
            // if the average error is + then left is faster
            average_error = accumulated_error / num_cycles;

            error_diff = error - last_error;





            // constants that terms are multipled by
            float Kp = 5;
            float Ki = .95; // don't go above 1.1 otherwise there is integral winding
            float Kd = .45;
            float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

            lcd_printf("Ramp Up\nLeft: %d \nRight: %d", sum_left, sum_right);


            int left_speed = current_speed - (int)multiply_factor;
            int right_speed = current_speed + (int)multiply_factor;



            // Make sure the speed doesn't go above 100 or below 0
            if(left_speed > 100) {
                left_speed = 100;
            }
            if(left_speed < 0) {
                left_speed = 0;
            }
            if(right_speed > 100) {
                right_speed = 100;
            }
            if(right_speed < 0) {
                right_speed = 0;
            }

            oi_setWheels(right_speed, left_speed);

            last_error = error;

            last_left = sensor_data->leftEncoderCount;
            last_right = sensor_data->rightEncoderCount;

            num_cycles += 1;


        }
            }
    ramp_up_difference = sum_left - sum_right;
    }







// Given a speed and a distance(in meters), increase the speed to that distance at a ceratin rate
// ramps down to 0
// speed is current speed
void ramp_down(oi_t *sensor_data, float distance, int speed) {
    int mmdistance = distance * 500;


    float distance_interval, speed_interval; // Every distance interval increase speed by a certain amount

    distance_interval = mmdistance / 5; // Have 6 points in the ramp up function where you increase speed
    speed_interval = (speed - 25) / 5; // want 25 to be the minimum speed



    float total_num_ticks = (mmdistance / 226.2) * 508.8;

    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = 0;
    int sum_right = 0;
    float left_diff = 0;
    float right_diff = 0;


    // used to sum up all of the error
    int num_cycles = 1;
    float accumulated_error = 0;
    float average_error = 0;
    float error_diff = 0;
    float last_error = 0;



    accumulated_error += (left_diff - right_diff);

    // if the average error is - then right is faster
    // if the average error is + then left is faster
    average_error = accumulated_error / num_cycles;

    // ramp down the speed 6 times
    int i;
    int current_speed = speed; // Speed that will be used to track current speed
    for(i = 5; i > 0; i--) {
        current_speed -= speed_interval;
        oi_setWheels(current_speed, current_speed);

        while((sum_left < total_num_ticks / i) && (sum_right < total_num_ticks / i)) {


            oi_update(sensor_data);

            left_diff = sensor_data->leftEncoderCount - last_left;
            right_diff = sensor_data->rightEncoderCount - last_right;

            // Total distance
            sum_left += left_diff;
            sum_right += right_diff;

            float error = left_diff - right_diff;
            accumulated_error += (left_diff - right_diff);
            // if the average error is - then right is faster
            // if the average error is + then left is faster
            average_error = accumulated_error / num_cycles;

            error_diff = error - last_error;





            // constants that terms are multipled by
            float Kp = 5;
            float Ki = .95; // don't go above 1.1 otherwise there is integral winding
            float Kd = .45;
            float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

            lcd_printf("Ramp Down\nLeft: %d \nRight: %d", sum_left, sum_right);


            int left_speed = current_speed - (int)multiply_factor;
            int right_speed = current_speed + (int)multiply_factor;



            // Make sure the speed doesn't go above 100 or below 0
            if(left_speed > 100) {
                left_speed = 100;
            }
            if(left_speed < 0) {
                left_speed = 0;
            }
            if(right_speed > 100) {
                right_speed = 100;
            }
            if(right_speed < 0) {
                right_speed = 0;
            }

            oi_setWheels(right_speed, left_speed);

            last_error = error;

            last_left = sensor_data->leftEncoderCount;
            last_right = sensor_data->rightEncoderCount;

            num_cycles += 1;
        }
    }
    ramp_down_difference = sum_left - sum_right;

    oi_setWheels(0, 0); // Make sure wheels are not moving


}
void ramp_down_left(oi_t *sensor_data, float degrees, int speed){
    // Total distance travled is (degrees/360) * Wheel_CIRCUMFERANCE
    lcd_init();
    lcd_clear();
    int speed_interval = speed / 4;
    float distance_interval = degrees / 4;
    float mmdistance = (degrees/360.0) * ROBOT_CIRCUMFERANCE;

    float total_num_ticks = ((mmdistance / 226.2) * 508.8) / 4;

    oi_update(sensor_data);
    int last_left = sensor_data->leftEncoderCount;
    int last_right = sensor_data->rightEncoderCount;
    int sum_left = 0;
    int sum_right = 0;
    float left_diff = 0;
    float right_diff = 0;

    // used to sum up all of the error
    int num_cycles = 1;
    float accumulated_error = 0;
    float error_diff = 0;
    float last_error = 0;

    int left_speed;
    int right_speed;

    int current_speed = speed;

    // constants that terms are multipled by
    float Kp = 1;
    float Ki = .1; // don't go above 1.1 otherwise there is integral winding
    float Kd = .6;


    int i;

    for(i = 0; i < 4; i++) {

    // Only want to go 9/10 of the circumferance then want to ramp down
    while (((sum_left + sum_right) / 2.0) < total_num_ticks) {

        oi_update(sensor_data);


        left_diff = sensor_data->leftEncoderCount - last_left;
        right_diff = sensor_data->rightEncoderCount - last_right;

        last_left = sensor_data->leftEncoderCount;
        last_right = sensor_data->rightEncoderCount;

        // Total distance
        sum_left += abs(left_diff);
        sum_right += abs(right_diff);

        float error = sum_left - sum_right;
        accumulated_error += error;
        // if the average error is - then right is faster
        // if the average error is + then left is faster


        error_diff = error - last_error;


        // Need to change the PID variables if it's the last loop becuaes of the speed
        if(i == 3) {
            Kp = 0.5;
            Ki = .05;
            Kd = .3;
        }

        float multiply_factor = (error * Kp) + (accumulated_error * Ki) + (error_diff * Kd); // When just right/left it still goes to the right a little

        lcd_printf("Ramping Down\nLeft: %d \nRight: %d", sum_left, sum_right);


        left_speed = -current_speed + (int)multiply_factor;
        right_speed = current_speed + (int)multiply_factor;


        oi_setWheels(right_speed, left_speed);

        last_error = error;

        last_left = sensor_data->leftEncoderCount;
        last_right = sensor_data->rightEncoderCount;

        num_cycles += 1;


    }
    sum_left = 0;
    sum_right = 0;
    current_speed -= speed_interval;
    }


    oi_setWheels(0, 0);
}
void ramp_down_right(oi_t *sensor_data, float distance, int speed){

}


