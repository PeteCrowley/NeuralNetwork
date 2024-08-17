#ifndef NETWORK_H
#define NETWORK_H

#include <random>
#include <algorithm>
#include <string>

#include "Layer.h"
#include "CostFunction.h"

using namespace std;

/**
 * Class for a neural network
*/
class Network{
private:
    vector<Layer> layers;      // Stores the layers of the network
    size_t num_inputs;         // Number of inputs to the network
    size_t num_outputs;        // Number of outputs from the network
    size_t num_layers;         // Number of layers in the network
    float learn_rate;      // Learning rate of the network
    int batch_size;         // Batch size for learning
    int epoch;              // Epochs run of the network
    int epoch_decay_rate;   // Number of epochs before the learning rate is halved
    CostFunction costFunction;  // Cost function of the network
    std::mt19937 rng;

public:
    Network();
    Network(Layer layer);
    Network(vector<Layer> layers, float learn_rate = 0.1, int epochs_per_decay = 5, int batch_size = 32, CostFunction costFunction = errorSquared);
    int classify(vector<float> inputs);
    string toString();
    vector<float> getOutput(vector<float> inputs);
    vector<vector<float>> getOutputs(vector<vector<float>> inputs);
    float cost(vector<float> inputs, vector<float> expected_output);
    float averageCost(vector<vector<float>> inputs, vector<vector<float>> expected_outputs);
    float accuracy(vector<vector<float>> inputs, vector<int> expected_integer_outputs);
    float accuracy(vector<vector<float>> inputs, vector<vector<float>> expected_outputs);
    void updateDerivatives(vector<float> input, vector<float> expected_output);
    void quickLearn(vector<vector<float>> inputs, vector<vector<float>> expected_outputs);
    void applyDerivatives(int batch_size);
    void learnWithBatchSize(vector<vector<float>> inputs, vector<vector<float>> expected_outputs);
    void train(vector<vector<float>> inputs, vector<vector<float>> expected_outputs, int epochs);
    vector<float> intToVector(int num);
    int getEpoch();
};

#endif // NETWORK_H