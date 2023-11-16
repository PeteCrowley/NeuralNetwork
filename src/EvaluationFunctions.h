#ifndef EVALUATION_FUNCTIONS_H
#define EVALUATION_FUNCTIONS_H

/**
 * This class contains static evaluation functions that are used to classify points
*/
class EvaluationFunctions {
    public:
        static int three_linear_sections(double x, double y);
        static int four_squares(double x, double y);
        static int cubic_function(double x, double y);
        static int quadratic_function(double x, double y);
        static int circle_function(double x, double y);
        static int three_class_circle(double x, double y);
        static int four_class_circle(double x, double y);
        static int tanh_function(double x, double y);
};

#endif // EVALUATION_FUNCTIONS_H
