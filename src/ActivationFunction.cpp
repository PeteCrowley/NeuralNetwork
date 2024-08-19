#include "ActivationFunction.h"
#include <math.h>
#include <vector>

using namespace std;

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
ActivationFunction::ActivationFunction(vector<float> (*activation)(vector<float> x), vector<float> (*derivative)(vector<float> x))
{
    this->activation = activation;
    this->derivative = derivative;
}

float sig_f(float x){
    return 1 / (1 + exp(-x));
}
/**
 * The sigmoid activation function
 * @param x: The value to be activated
 * @return: The activated value
 */
vector<float> sig(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = sig_f(x[i]);
    }
    return return_result;
}

/**
 * The derivative of the sigmoid activation function
 * @param x: The value to be activated
 * @return: The derivative of the activated value
 */
vector<float> sigDeriv(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = sig_f(x[i]) * (1 - sig_f(x[i]));
    }
    return return_result;
}

ActivationFunction sigmoid = ActivationFunction(sig, sigDeriv);

/**
 * The tanH activation function
 * @param x: The value to be activated
 * @return: The activated value
 */
vector<float> tanHDec(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = tanh(x[i]);
    }
    return return_result;
}

/**
 * The derivative of the tanH activation function
 * @param x: The value to be activated
 * @return: The derivative of the activated value
 */
vector<float> tanHDeriv(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = 1 - tanh(x[i]) * tanh(x[i]);
    }
    return return_result;
}

ActivationFunction tanH = ActivationFunction(tanHDec, tanHDeriv);

/**
 * The ReLu activation function
 * @param x: The value to be activated
 * @return: The activated value
 */
vector<float> reLu(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = x[i] > 0 ? x[i] : 0;
    }
    return return_result;
}

/**
 * The derivative of the ReLu activation function
 * @param x: The value to be activated
 * @return: The derivative of the activated value
 */
vector<float> reluDeriv(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = x[i] > 0 ? 1 : 0;
    }
    return return_result;
}

ActivationFunction relu = ActivationFunction(reLu, reluDeriv);

/**
 * The identity activation function
 * @param x: The value to be activated
 * @return: The activated value
 */
vector<float> id(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = x[i];
    }
    return return_result;
}

/**
 * The derivative of the identity activation function
 * @param x: The value to be activated
 * @return: The derivative of the activated value
 */
vector<float> identityDeriv(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    for (int i = 0; i < x.size(); i++){
        return_result[i] = 1;
    }
    return return_result;
}

ActivationFunction identity = ActivationFunction(id, identityDeriv);


vector<float> softMax(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    // Find maximum
    float max = x[0];
    for (int i = 1; i < x.size(); i++){
        if (x[i] > max){
            max = x[i];
        }
    }
    float sum = 0;
    for (int i = 0; i < x.size(); i++){
        return_result[i] = exp(x[i] - max);
        sum += return_result[i];
    }
    for (int i = 0; i < x.size(); i++){
        return_result[i] /= sum;
    }
    return return_result;
}

vector<float> softMaxDeriv(vector<float> x){
    vector<float> return_result = vector<float>(x.size());
    vector<float> softMaxX = softMax(x);
    for (int i = 0; i < x.size(); i++){
        return_result[i] = softMaxX[i] * (1 - softMaxX[i]);
    }
    return return_result;

}

ActivationFunction softmax = ActivationFunction(softMax, softMaxDeriv);
