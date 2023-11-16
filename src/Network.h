#ifndef NETWORK_H
#define NETWORK_H

#include "Layer.h"
#include "CostFunction.h"

using namespace std;

/**
 * Class for a neural network
*/
class Network{
private:
    vector<Layer> layers;      // Stores the layers of the network
    int num_inputs;         // Number of inputs to the network
    int num_outputs;        // Number of outputs from the network
    int num_layers;         // Number of layers in the network
    double learn_rate;      // Learning rate of the network
    int batch_size;         // Batch size for learning
    int epoch;              // Epochs run of the network
    int epoch_decay_rate;   // Number of epochs before the learning rate is halved
    CostFunction costFunction;  // Cost function of the network


public:
    Network();
    Network(Layer layer);
    Network(vector<Layer> layers, double learn_rate = 0.1, int epochs_per_decay = 5, int batch_size = 32, CostFunction costFunction = errorSquared);
    int classify(vector<double> inputs);
    string toString();
    vector<double> getOutput(vector<double> inputs);
    vector<vector<double>> getOutputs(vector<vector<double>> inputs);
    double cost(vector<double> inputs, vector<double> expected_output);
    double averageCost(vector<vector<double>> inputs, vector<vector<double>> expected_outputs);
    double accuracy(vector<vector<double>> inputs, vector<int> expected_outputs);
    void updateDerivatives(vector<double> input, vector<double> expected_output);
    void quickLearn(vector<vector<double>> inputs, vector<vector<double>> expected_outputs);
    void applyDerivatives(int batch_size);
    void learnWithBatchSize(vector<vector<double>> inputs, vector<vector<double>> expected_outputs);
    vector<double> intToVector(int num);
    int getEpoch();
};

#endif // NETWORK_H