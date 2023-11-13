#ifndef LAYER_H
#define LAYER_H

#include <vector>

class Layer {
private:
    int num_inputs;                     // Number of inputs to the layer
    int num_outputs;                    // Number of outputs from the layer
    std::vector<std::vector<double>> weights;     // Stores the weights with indexes [output][input]
    std::vector<double> biases;              // Stores the biases for each neuron

    static bool seeded;
    double (*activationFunction)(double x);

public:
    Layer(int num_neurons, int num_neuron);     // Constructor
    Layer(int num_neurons, int num_neuron, double (*activationFunction)(double x));
    std::vector<double> calculateOutputs(std::vector<double> inputs);  // Calculates the outputs of the layer
    std::string toString();
    int getNumInputs();
    int getNumOutputs();
    double getWeight(int neuronIndex, int inputIndex);
    double getBias(int neuronIndex);
    void setNeuronWeight(int neuronIndex, int inputIndex, double weight);
    void setNeuronBias(int neuronIndex, double bias);
};

#endif // LAYER_H
