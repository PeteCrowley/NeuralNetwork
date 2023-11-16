#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

/**
 * Class for an activation function
*/
class ActivationFunction {
public:
    /**
     * Returns the activation of a certain value
     * @param x: The value to be activated
     * @return: The activated value
    */
    double (*activation)(double x);
    /**
     * Returns the derivative of the activation function for a certain value
     * @param x: The value to be activated
     * @return: The derivative of the activated value
    */
    double (*derivative)(double x);
    ActivationFunction();
    ActivationFunction(double (*activationFunction)(double x), double (*derivative)(double x));
};

extern ActivationFunction sigmoid;
extern ActivationFunction tanH;
extern ActivationFunction relu;
extern ActivationFunction identity;


#endif // ACTIVATION_FUNCTION_H