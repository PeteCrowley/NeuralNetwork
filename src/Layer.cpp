#include <iostream>
#include <vector>
#include <random>
#include "Layer.h"
#include "ActivationFunction.h"

using namespace std;

bool Layer::seeded = false;

Layer::Layer(int num_inputs, int num_outputs) : Layer(num_inputs, num_outputs, identity){} // default activation function is the identity function (f(x) = x

Layer::Layer(int num_inputs, int num_outputs, ActivationFunction activationFunction){
    this-> num_inputs = num_inputs;             
    this-> num_outputs = num_outputs;
    this-> activationFunction = activationFunction;

    // Seed the random number generator
    if (!seeded){
        srandom(time(NULL));
        seeded = true;
    }
    
    // Initialize the weights and biases to random values                   
    for (int outputIndex = 0; outputIndex < num_outputs; outputIndex++)
    {
        
        double randomBias = ((double)random() / RAND_MAX) * 2 - 1;   // Random bias
        vector<double> neuron_weights;
        for (int inputIndex = 0; inputIndex < num_inputs; inputIndex++)
        {
            neuron_weights.insert(neuron_weights.end(), (double)random() / RAND_MAX * 2 - 1);     // Random weights
        }
        weights.push_back(neuron_weights);
        biases.push_back(randomBias);
    }
}

vector<double> Layer::calculateOutputs(vector<double> inputs){
    vector<double> outputs;

    // calculate each output one at a time
    for (int outputIndex = 0; outputIndex < num_outputs; outputIndex++)
    {
        double output = biases[outputIndex];        // start with the bias
        // loop through each input and add the weighted input
        for (int inputIndex = 0; inputIndex < num_inputs; inputIndex++)
        {
            output += inputs[inputIndex] * weights[outputIndex][inputIndex];
        }
        outputs.insert(outputs.end(), output);      // add the output to our vector
    }
    for(int i = 0; i < outputs.size(); i++){
        outputs[i] = activationFunction.activationFunction(outputs[i]);
    }
    return outputs;
}

string Layer::toString(){
    string output = "";
    for(int neuronIndex = 0; neuronIndex < num_outputs; neuronIndex++){
        output += "Neuron " + to_string(neuronIndex) + ": " + to_string(biases[neuronIndex]) + " + ";
        for (int inputIndex = 0; inputIndex < num_inputs - 1; inputIndex++){
            output += to_string(weights[neuronIndex][inputIndex]) + "x" + to_string(inputIndex) + " + ";

        }
        output += to_string(weights[neuronIndex][num_inputs - 1]) + "x" + to_string(num_inputs - 1);
        output += "\n";
    }
    return output;
}

// Implement getters I just defined
int Layer::getNumInputs(){
    return num_inputs;
}

int Layer::getNumOutputs(){
    return num_outputs;
}

double Layer::getWeight(int neuronIndex, int inputIndex){
    return this->weights[neuronIndex][inputIndex];
}

double Layer::getBias(int neuronIndex){
    return this->biases[neuronIndex];
}

void Layer::setNeuronWeight(int neuronIndex, int inputIndex, double weight){
    this->weights[neuronIndex][inputIndex] = weight;
}

void Layer:: setNeuronBias(int neuronIndex, double bias){
    this->biases[neuronIndex] = bias;
}
