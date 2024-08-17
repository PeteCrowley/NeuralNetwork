#ifndef EVALUATION_FUNCTIONS_H
#define EVALUATION_FUNCTIONS_H

#include <map>
#include <string>

/**
 * This class contains static evaluation functions that are used to classify points
*/
using EvalFunctionPtr = int (*)(float, float);

class EvaluationFunctions {
    public:
        static int three_linear_sections(float x, float y);
        static int four_squares(float x, float y);
        static int cubic_function(float x, float y);
        static int quadratic_function(float x, float y);
        static int circle_function(float x, float y);
        static int three_class_circle(float x, float y);
        static int four_class_circle(float x, float y);
        static int tanh_function(float x, float y);
        const static std::map<std::string, EvalFunctionPtr> function_map;
        const static std::map<std::string, int> num_classes_map;
};


#endif // EVALUATION_FUNCTIONS_H
