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
    float (*activation)(float x);
    /**
     * Returns the derivative of the activation function for a certain value
     * @param x: The value to be activated
     * @return: The derivative of the activated value
    */
    float (*derivative)(float x);
    ActivationFunction();
    ActivationFunction(float (*activationFunction)(float x), float (*derivative)(float x));
};

extern ActivationFunction sigmoid;
extern ActivationFunction tanH;
extern ActivationFunction relu;
extern ActivationFunction identity;

#endif // ACTIVATION_FUNCTION_H