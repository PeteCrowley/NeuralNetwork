#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include <string>
#include "ActivationFunction.h"
#include "CostFunction.h"

using namespace std;

/**
 * Class for a layer in a neural network
*/
class Layer {

private:
    size_t num_inputs;                     // Number of inputs to the layer
    size_t num_outputs;                    // Number of outputs from the layer
    vector<vector<float>> weights;     // Stores the weights with indexes [output][input]
    vector<float> biases;              // Stores the biases for each neuron


    static bool seeded;
    ActivationFunction activationFunction;

    // Variables for backpropagation
    vector<float> previousInputs;
    vector<float> previousOutputs;
    vector<float> previousActivations;
    vector<float> derivativesCostRespectToOutputs;
    vector<vector<float>> weightDerivatives;
    vector<float> biasDerivatives;


public:
    Layer(int num_neurons, int num_neuron);     // Constructor
    Layer(int num_neurons, int num_neuron, ActivationFunction activationFunction);
    vector<float> calculateOutputs(vector<float> inputs);  // Calculates the outputs of the layer
    string toString();
    size_t getNumInputs();
    size_t getNumOutputs();
    float getWeight(int neuronIndex, int inputIndex);
    float getBias(int neuronIndex);
    void setNeuronWeight(int neuronIndex, int inputIndex, float weight);
    void setNeuronBias(int neuronIndex, float bias);
    void calculateOutputLayerPartialDerivatives(CostFunction costFunction, vector<float> expectedOutputs);
    void calculateHiddenLayerPartialDerivatives(Layer nextLayer);
    void calculateBiasPartialDerivatives(Layer nextLayer);
    vector<vector<float>> getWeightDerivatives();
    vector<float> getBiasDerivatives();
    void resetDerivatives();
};

#endif // LAYER_H
