#include "ActivationFunction.h"
#include <math.h>

/**
 * Default constructor for ActivationFunction
*/
ActivationFunction::ActivationFunction()
{
    this->activation = nullptr;
    this->derivative = nullptr;
}

/**
 * Constructor for ActivationFunction
 * @param activation: The activation function
 * @param derivative: The derivative of the activation function
*/
ActivationFunction::ActivationFunction(double (*activation)(double x), double (*derivative)(double x))
{
    this->activation = activation;
    this->derivative = derivative;
}

double sig(double x){
    return 1 / (1 + exp(-x));
}

double sigDeriv(double x){
    return sig(x) * (1 - sig(x));
}

ActivationFunction sigmoid = ActivationFunction(sig, sigDeriv);


double tanHDec(double x){
    return tanh(x);
}

double tanHDeriv(double x){
    return 1 - tanh(x) * tanh(x);
}

ActivationFunction tanH = ActivationFunction(tanHDec, tanHDeriv);

double reLu(double x){
    return x > 0 ? x : 0;
}

double reluDeriv(double x){
    return x > 0 ? 1 : 0;
}

ActivationFunction relu = ActivationFunction(reLu, reluDeriv);

double id(double x){
    return x;
}

double identityDeriv(double x){
    return 1;
}

ActivationFunction identity = ActivationFunction(id, identityDeriv);


