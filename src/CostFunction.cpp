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
CostFunction::CostFunction(float (*cost)(float output, float expectedOutput), float (*derivative)(float output, float expectedOutput)){
    this->cost = cost;
    this->derivative = derivative;
}

float errorSquaredCost(float output, float expectedOutput){
    return (output - expectedOutput) * (output - expectedOutput);
}

float errorSquaredDeriv(float output, float expectedOutput){
    return 2 * (output - expectedOutput);
}

CostFunction errorSquared = CostFunction(errorSquaredCost, errorSquaredDeriv);
