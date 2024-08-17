#include <math.h>
#include <string>
#include "EvaluationFunctions.h"


/**
* Divides the data points into three linear sections on the screen
* Recommended MAX_POINT_VALUE: 5-10
* Number of classes: 3
*/
int EvaluationFunctions::three_linear_sections(float x, float y){
    float diff =  x - y;
    if (diff < -1){
        return 2;
    }
    else if (diff < 2){
        return 1;
    }
    else {
        return 0;
    }
}

/**
* Divides the data points into four square sections
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 4
*/
int EvaluationFunctions::four_squares(float x, float y){
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
int EvaluationFunctions::cubic_function(float x, float y){
    return (2*x-1)*(2*x-1)*(2*x-1) + 0.5< y;
}

/**
* Seperates the data points via a quadtaric function
* Recommended MAX_POINT_VALUE: 1
* Number of classes: 2
*/
int EvaluationFunctions::quadratic_function(float x, float y){
    return x * x < y;
}

/**
* Divides the data points based on whether they are inside a circle
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 2
*/
int EvaluationFunctions::circle_function(float x, float y){
    int middle_x = 5;
    int middle_y = 5;
    return (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y) < 9;
}

/**
* Seperates the data points into 3 classes based on distance from the center
* Recommended MAX_POINT_VALUE: 10
* Number of classes: 3
*/
int EvaluationFunctions::three_class_circle(float x, float y){
    int middle_x = 5;
    int middle_y = 5;
    float distance = (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y);
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
int EvaluationFunctions::four_class_circle(float x, float y){
    int middle_x = 5;
    int middle_y = 5;
    float distance = (x-middle_x)*(x-middle_x) + (y-middle_y)*(y-middle_y);
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
int EvaluationFunctions::tanh_function(float x, float y){
    return 4 * tanh(x-4) + 5 < y;
}

/**
 * Map of evaluation function names to function pointers
 */
const std::map<std::string, EvalFunctionPtr> EvaluationFunctions::function_map = {
    {"linear", EvaluationFunctions::three_linear_sections},
    {"4_squares", EvaluationFunctions::four_squares},
    {"cubic", EvaluationFunctions::cubic_function},
    {"quadratic", EvaluationFunctions::quadratic_function},
    {"circle", EvaluationFunctions::circle_function},
    {"3circles", EvaluationFunctions::three_class_circle},
    {"4circles", EvaluationFunctions::four_class_circle},
    {"tanh", EvaluationFunctions::tanh_function},
};

/**
 * Map of evaluation function names to number of classes of data points
 */
const std::map<std::string, int> EvaluationFunctions::num_classes_map = {
    {"3linear", 3},
    {"4squares", 4},
    {"cubic", 2},
    {"quadratic", 2},
    {"circle", 2},
    {"3circles", 3},
    {"4circles", 4},
    {"tanh", 2},
};
