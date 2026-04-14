#include "CostFunction.h"
#include <math.h>

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

/**
 * Calculates the cost of the output based on the error squared algorithm
 * @param output: The output of the network
 * @param expectedOutput: The expected output of the network
 * @return: The cost of the output
*/
float errorSquaredCost(float output, float expectedOutput){
    return (output - expectedOutput) * (output - expectedOutput);
}

/**
 * The derivative of the errorSquaredCost function
 * @param output: The output of the network
 * @param expectedOutput: The expected output of the network
 * @return: The derivative of the cost of the output
*/
float errorSquaredDeriv(float output, float expectedOutput){
    return 2 * (output - expectedOutput);
}

CostFunction errorSquared = CostFunction(errorSquaredCost, errorSquaredDeriv);

/**
 * Calculates the (per-class) cross entropy cost contribution.
 * Expected output is assumed to be one-hot or probability target.
*/
float crossEntropyCost(float output, float expectedOutput){
    const float epsilon = 1e-7f;
    float clampedOutput = output;
    if (clampedOutput < epsilon) {
        clampedOutput = epsilon;
    }
    if (clampedOutput > 1.0f - epsilon) {
        clampedOutput = 1.0f - epsilon;
    }
    return -expectedOutput * log(clampedOutput);
}

/**
 * Derivative of (per-class) cross entropy w.r.t. activation output.
*/
float crossEntropyDeriv(float output, float expectedOutput){
    const float epsilon = 1e-7f;
    float clampedOutput = output;
    if (clampedOutput < epsilon) {
        clampedOutput = epsilon;
    }
    if (clampedOutput > 1.0f - epsilon) {
        clampedOutput = 1.0f - epsilon;
    }
    return -expectedOutput / clampedOutput;
}

CostFunction crossEntropy = CostFunction(crossEntropyCost, crossEntropyDeriv);
