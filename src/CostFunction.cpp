#include "CostFunction.h"

/**
 * Default constructor for CostFunction
*/
CostFunction::CostFunction(){
    this->cost = nullptr;
    this->derivative = nullptr;
}

/**
 * Constructor for CostFunction
 * @param cost: The cost function
 * @param derivative: The derivative of the cost function
*/
CostFunction::CostFunction(double (*cost)(double output, double expectedOutput), double (*derivative)(double output, double expectedOutput)){
    this->cost = cost;
    this->derivative = derivative;
}

double errorSquaredCost(double output, double expectedOutput){
    return (output - expectedOutput) * (output - expectedOutput);
}

double errorSquaredDeriv(double output, double expectedOutput){
    return 2 * (output - expectedOutput);
}

CostFunction errorSquared = CostFunction(errorSquaredCost, errorSquaredDeriv);
