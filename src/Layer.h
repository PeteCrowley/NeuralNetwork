#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "ActivationFunction.h"
#include "CostFunction.h"

using namespace std;

/**
 * Class for a layer in a neural network
*/
class Layer {

private:
    int num_inputs;                     // Number of inputs to the layer
    int num_outputs;                    // Number of outputs from the layer
    vector<vector<double>> weights;     // Stores the weights with indexes [output][input]
    vector<double> biases;              // Stores the biases for each neuron


    static bool seeded;
    ActivationFunction activationFunction;

    // Variables for backpropagation
    vector<double> previousInputs;
    vector<double> previousOutputs;
    vector<double> previousActivations;
    vector<double> derivatvesCostRespectToOutputs;
    vector<vector<double>> weightDerivatives;
    vector<double> biasDerivatives;


public:
    Layer(int num_neurons, int num_neuron);     // Constructor
    Layer(int num_neurons, int num_neuron, ActivationFunction activationFunction);
    vector<double> calculateOutputs(vector<double> inputs);  // Calculates the outputs of the layer
    string toString();
    int getNumInputs();
    int getNumOutputs();
    double getWeight(int neuronIndex, int inputIndex);
    double getBias(int neuronIndex);
    void setNeuronWeight(int neuronIndex, int inputIndex, double weight);
    void setNeuronBias(int neuronIndex, double bias);
    void calculateOutputLayerPartialDerivatives(CostFunction costFunction, vector<double> expectedOutputs);
    void calculateHiddenLayerPartialDerivatives(Layer nextLayer);
    void calculateBiasPartialDerivatives(Layer nextLayer);
    vector<vector<double>> getWeightDerivatives();
    vector<double> getBiasDerivatives();
    void resetDerivatives();
};

#endif // LAYER_H
