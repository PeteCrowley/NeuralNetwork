#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H


#include <vector>

using namespace std;

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
    vector<float> (*activation)(vector<float> x);
    /**
     * Returns the derivative of the activation function for a certain value
     * @param x: The value to be activated
     * @return: The derivative of the activated value
    */
    vector<float> (*derivative)(vector<float> x);
    ActivationFunction();
    ActivationFunction(vector<float> (*activationFunction)(vector<float> x), vector<float> (*derivative)(vector<float> x));
};

extern ActivationFunction sigmoid;
extern ActivationFunction tanH;
extern ActivationFunction relu;
extern ActivationFunction identity;
extern ActivationFunction softmax;

#endif // ACTIVATION_FUNCTION_H