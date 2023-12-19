
#include <colour_utils.h>


int sweep(int input_val, int min, int max, int increment) {
    if (increment <= 0) {
        printf("Increment must be greater than 0\n");
        return input_val;
    }
    if (input_val < min) {
        input_val = min;
    } else if (input_val < max) {
        input_val += increment;
    } else {
        input_val = min;
    }
    return input_val;
}

float sweep(float input_val, float min, float max, float increment) {
    if (increment <= 0) {
        printf("Increment must be greater than 0\n");
        return input_val;
    }
    if (input_val < min) {
        input_val = min;
    } else if (input_val < max) {
        input_val += increment;
    } else {
        input_val = min;
    }
    return input_val;
}


std::pair<int, int> bounce(int input_val, int last_direction, int min, int max, int increment) {
    if (input_val < max && last_direction == 1) {
        input_val += increment;
    } else if (input_val >= max && last_direction == 1) {
        input_val -= increment;
        last_direction = -1;
    } else if (input_val > min && last_direction == -1) {
        input_val -= increment;
    } else if (input_val <= min && last_direction == -1) {
        input_val += increment;
        last_direction = 1;
    }
    return std::make_pair(input_val, last_direction);
}


std::pair<float, int> bounce(float input_val, int last_direction, float min, float max, float increment) {
    if (input_val < max && last_direction == 1) {
        input_val += increment;
    } else if (input_val >= max && last_direction == 1) {
        input_val -= increment;
        last_direction = -1;
    } else if (input_val > min && last_direction == -1) {
        input_val -= increment;
    } else if (input_val <= min && last_direction == -1) {
        input_val += increment;
        last_direction = 1;
    }
    return std::make_pair(input_val, last_direction);
}

