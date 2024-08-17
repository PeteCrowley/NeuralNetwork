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
ActivationFunction::ActivationFunction(float (*activation)(float x), float (*derivative)(float x))
{
    this->activation = activation;
    this->derivative = derivative;
}

float sig(float x){
    return 1 / (1 + exp(-x));
}

float sigDeriv(float x){
    return sig(x) * (1 - sig(x));
}

ActivationFunction sigmoid = ActivationFunction(sig, sigDeriv);


float tanHDec(float x){
    return tanh(x);
}

float tanHDeriv(float x){
    return 1 - tanh(x) * tanh(x);
}

ActivationFunction tanH = ActivationFunction(tanHDec, tanHDeriv);

float reLu(float x){
    return x > 0 ? x : 0;
}

float reluDeriv(float x){
    return x > 0 ? 1 : 0;
}

ActivationFunction relu = ActivationFunction(reLu, reluDeriv);

float id(float x){
    return x;
}

float identityDeriv(float x){
    return 1;
}

ActivationFunction identity = ActivationFunction(id, identityDeriv);


