#include <math.h>
#include "EvaluationFunctions.h"

/**
* Divides the data points into three linear sections on the screen
* Recommended MAX_POINT_VALUE: 5-10
* Number of classes: 3
*/
int EvaluationFunctions::three_linear_sections(double x, double y){
    double diff =  x - y;
    if (diff < -1){
        return 0;
    }
    else if (diff < 2){
        return 1;
    }
    else {
        return 2;
    }
}

/**
* Divides the data points into four square sections
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 4
*/
int EvaluationFunctions::four_squares(double x, double y){
    int pivot = 5;
    x = x - pivot;
    y = y - pivot;
    if (x < 0 && y < 0){
        return 0;
    }
    else if (x < 0 && y > 0){
        return 1;
    }
    else if (x > 0 && y < 0){
        return 2;
    }
    else {
        return 3;
    }
}

/**
* Seperates data via a cubic function
* Recommended MAX_POINT_VALUE: 1
* Number of classes: 2
*/
int EvaluationFunctions::cubic_function(double x, double y){
    return (2*x-1)*(2*x-1)*(2*x-1) + 0.5< y;
}

/**
* Seperates the data points via a quadtaric function
* Recommended MAX_POINT_VALUE: 1
* Number of classes: 2
*/
int EvaluationFunctions::quadratic_function(double x, double y){
    return x * x < y;
}

/**
* Divides the data points based on whether they are inside a circle
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 2
*/
int EvaluationFunctions::circle_function(double x, double y){
    int middle_x = 5;
    int middle_y = 5;
    return (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y) < 9;
}

/**
* Seperates the data points into 3 classes based on distance from the center
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 3
*/
int EvaluationFunctions::three_class_circle(double x, double y){
    int middle_x = 5;
    int middle_y = 5;
    double distance = (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y);
    if (distance < 9){
        return 0;
    }
    else if ((x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y) < 16){
        return 1;
    }
    else {
        return 2;
    }
}

/**
* Seperates the data points into 4 classes based on distance from the center
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 4
*/
int EvaluationFunctions::four_class_circle(double x, double y){
    int middle_x = 5;
    int middle_y = 5;
    double distance = (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y);
    if (distance < 4){
        return 0;
    }
    else if (distance < 9){
        return 1;
    }
    else if ((x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y) < 16){
        return 3;
    }
    else {
        return 2;
    }
}

/**
* Seperates the data points via a tanh function
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 2
*/
int EvaluationFunctions::tanh_function(double x, double y){
    return 4 * tanh(x-4) + 5 < y;
}