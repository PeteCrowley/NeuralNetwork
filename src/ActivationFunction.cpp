#include "ActivationFunction.h"

ActivationFunction::ActivationFunction()
{
    this->activationFunction = nullptr;
    this->derivative = nullptr;
}

ActivationFunction::ActivationFunction(double (*activationFunction)(double x), double (*derivative)(double x))
{
    this->activationFunction = activationFunction;
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
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

double tanHDeriv(double x){
    return 1 - tanHDec(x) * tanHDec(x);
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


