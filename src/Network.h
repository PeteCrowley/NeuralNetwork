#ifndef NETWORK_H
#define NETWORK_H

#include "Layer.h"

using namespace std;

class Network{
private:
    int num_inputs;                 // Number of inputs to the network
    int num_outputs;                // Number of outputs from the network
    int num_layers;                 // Number of layers in the network
    double learn_rate;
    int batch_size;
    int epoch;
    int epoch_decay_rate;


public:
    vector<Layer> layers;      // Stores the layers of the network
    Network();
    Network(int num_inputs, int num_outputs, Layer layer);
    Network(int num_inputs, int num_outputs, vector<Layer> layers, double learn_rate = 0.1, int batch_size = 32);
    int classify(vector<double> inputs);
    string toString();
    vector<double> getOutput(vector<double> inputs);
    vector<vector<double>> getOutputs(vector<vector<double>> inputs);
    double cost(vector<double> inputs, vector<double> expected_output);
    double averageCost(vector<vector<double>> inputs, vector<vector<double>> outputs);
    void learn(vector<vector<double>> inputs, vector<vector<double>> outputs);
    void updateBiasesAndWeights(vector<vector<double>> biasDerivatives, vector<vector<vector<double>>> weightDerivatives);
    void learnWithBatchSize(vector<vector<double>> inputs, vector<vector<double>> outputs, int batch_size);
    vector<double> intToVector(int num);
    int getBatchSize();
    int getEpoch();
};

#endif // NETWORK_H