#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include <vector>

class ActivationFunction {
public:
    double (*activationFunction)(double x);
    double (*derivative)(double x);
    ActivationFunction();
    ActivationFunction(double (*activationFunction)(double x), double (*derivative)(double x));
};

extern ActivationFunction sigmoid;
extern ActivationFunction tanH;
extern ActivationFunction relu;
extern ActivationFunction identity;


#endif // ACTIVATION_FUNCTION_H