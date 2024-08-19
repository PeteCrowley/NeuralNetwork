#include <iostream>
#include <vector>
#include <random>
#include "Layer.h"
#include "ActivationFunction.h"
#include <omp.h>

using namespace std;

bool Layer::seeded = false;

/**
 * Constructor for class layer
 * @param num_inputs: The number of inputs to the layer
 * @param num_outputs: The number of outputs from the layer
 * Will use the identity activation function
*/
Layer::Layer(int num_inputs, int num_outputs) : Layer(num_inputs, num_outputs, identity){} // default activation function is the identity function (f(x) = x

/**
 * Constructor for class layer
 * @param num_inputs: The number of inputs to the layer
 * @param num_outputs: The number of outputs from the layer
 * @param activationFunction: The activation function of the layer
*/
Layer::Layer(int num_inputs, int num_outputs, ActivationFunction activationFunction){
    this-> num_inputs = num_inputs;             
    this-> num_outputs = num_outputs;
    this-> activationFunction = activationFunction;
    previousInputs = vector<float>(num_inputs);
    previousOutputs = vector<float>(num_outputs);
    previousActivations = vector<float>(num_outputs);
    derivativesCostRespectToOutputs = vector<float>(num_outputs);
    resetDerivatives();

    // Seed the random number generator
    if (!seeded){
        srandom(time(NULL));
        seeded = true;
    }
    
    // Initialize the weights and biases to random values                   
    for (int outputIndex = 0; outputIndex < num_outputs; outputIndex++)
    {
        float randomBias = ((float)random() / RAND_MAX) * 2 - 1;   // Random bias
        vector<float> neuron_weights;
        for (int inputIndex = 0; inputIndex < num_inputs; inputIndex++)
        {
            neuron_weights.insert(neuron_weights.end(), (float)random() / RAND_MAX * 2 - 1);     // Random weights
        }
        weights.push_back(neuron_weights);
        biases.push_back(randomBias);
    }
}

/**
 * Calculates the outputs of the layer and stores the inputs, outputs, and activations
 * @param inputs: The inputs to the layer
 * @return: The activations of the outputs of the layer
*/
vector<float> Layer::calculateOutputs(vector<float> inputs){
    previousInputs = inputs;

    vector<float> outputs(num_outputs, 0.0f);

    // calculate each output one at a time
    for (size_t outputIndex = 0; outputIndex < num_outputs; outputIndex++)
    {
        float output = biases[outputIndex];        // start with the bias
        // loop through each input and add the weighted input

        for (size_t inputIndex = 0; inputIndex < num_inputs; inputIndex++)
        {
            output += inputs[inputIndex] * weights[outputIndex][inputIndex];
        }
        outputs[outputIndex] = output;      // add the output to our vector
    }
    previousOutputs = outputs;
    outputs = activationFunction.activation(outputs);
    previousActivations = outputs;
    
    return outputs;
}

/**
 * Calculates the partial derivatives of the weights and biases if this is an output
 * Stores the weight derivatives in weightDerivatives and derivatves needed for future backpropogation in derivatvesCostRespectToOutputs
 * @param costFunction: The cost function of the network
 * @param expectedOutputs: The expected outputs for the layer
*/
void Layer::calculateOutputLayerPartialDerivatives(CostFunction costFunction, vector<float> expectedOutputs){
    // Calculate all of the partials of the weights
    vector<float> activationDerivatives = activationFunction.derivative(previousOutputs);
    // #pragma omp parallel for
    for(size_t outputIndex = 0; outputIndex<num_outputs; outputIndex++){
        // dc/dz = da/dz * dc/da
        float dAdz = activationDerivatives[outputIndex];
        float dCda = costFunction.derivative(previousActivations[outputIndex], expectedOutputs[outputIndex]);
        derivativesCostRespectToOutputs[outputIndex] = dCda * dAdz;      // store values for future layers
        // #pragma omp parallel for
        for(size_t inputIndex = 0; inputIndex < num_inputs; inputIndex++){
            // dc/dw = dz/dw * dc/dz
            float dZdw = previousInputs[inputIndex];
            float deriv = dZdw * derivativesCostRespectToOutputs[outputIndex];
            weightDerivatives[outputIndex][inputIndex] += deriv;
        }
    }
}


/**
 * Calculates the partial derivatives of the weights and biases if this is a hidden layer
 * Stores the weight derivatives in weightDerivatives and derivatves needed for future backpropogation in derivatvesCostRespectToOutputs
 * @param nextLayer: The next layer in the network
*/
void Layer::calculateHiddenLayerPartialDerivatives(Layer nextLayer)
{
    // Calculate all of the partials of the weights
    vector<float> activationDerivatives = activationFunction.derivative(previousOutputs);
    // #pragma omp parallel for
    for(size_t outputIndex = 0; outputIndex<num_outputs; outputIndex++){
        // dc/dz = da/dz * (dZnext/da * dc/dznext) for each neuron in next leyar
        float dAdz = activationDerivatives[outputIndex];
        float dCdznext = 0;
        for (size_t nextNeuronIndex = 0; nextNeuronIndex < nextLayer.getNumOutputs(); nextNeuronIndex++){
            dCdznext += nextLayer.getWeight(nextNeuronIndex, outputIndex) * nextLayer.derivativesCostRespectToOutputs[nextNeuronIndex];
        }
        derivativesCostRespectToOutputs[outputIndex] = dAdz * dCdznext;          // store values for future layers
        // #pragma omp parallel for
        for(size_t inputIndex = 0; inputIndex < num_inputs; inputIndex++){
            // dc/dw1 = dw1/dz1 * da1/dz1 * dz2/da1 * da2/dz2 * dc/da2
            // dc/dw1 = dw1/dz1 * da1/dz1 * dz2/da1 * dc/daz2
            // dc/dw = dz/dw * dc/dz
            float dZdw = previousInputs[inputIndex];
            float deriv = dZdw * derivativesCostRespectToOutputs[outputIndex];
            weightDerivatives[outputIndex][inputIndex] += deriv;
        }
    }
}

/**
 * Calculates the partial derivatives of the biases
 * Stores the bias derivatives in biasDerivatives
 * @param nextLayer: The next layer in the network
*/
void Layer::calculateBiasPartialDerivatives(Layer nextLayer)
{
    for (size_t outputIndex = 0; outputIndex < num_outputs; outputIndex++){
        // dc/db = dz/db * dc/dz
        // float dZdb = 1;
        biasDerivatives[outputIndex] += derivativesCostRespectToOutputs[outputIndex];
    }
}

/**
 * Resets the derivatives of the weights and biases to 0
*/
void Layer::resetDerivatives(){
    weightDerivatives = vector<vector<float>>(num_outputs, vector<float>(num_inputs));
    biasDerivatives = vector<float>(num_outputs);
}

/**
 * Returns the number of inputs to the layer
 * @return: The number of inputs to the layer
*/
size_t Layer::getNumInputs(){
    return num_inputs;
}

/**
 * Returns the number of outputs from the layer
 * @return: The number of outputs from the layer
*/
size_t Layer::getNumOutputs(){
    return num_outputs;
}

/**
 * Returns the weight of a neuron
 * @param neuronIndex: The index of the neuron
 * @param inputIndex: The index of the weight
 * @return: The specified weight of the neuron
*/
float Layer::getWeight(int neuronIndex, int inputIndex){
    return this->weights[neuronIndex][inputIndex];
}

/**
 * Returns the bias of a neuron
 * @param neuronIndex: The index of the neuron
 * @return: The bias of the neuron
*/
float Layer::getBias(int neuronIndex){
    return this->biases[neuronIndex];
}

/**
 * Sets the weight of a neuron at a specified index
 * @param neuronIndex: The index of the neuron
 * @param inputIndex: The index of the weight
 * @param weight: The new weight of the neuron
*/
void Layer::setNeuronWeight(int neuronIndex, int inputIndex, float weight){
    this->weights[neuronIndex][inputIndex] = weight;
}

/**
 * Sets the bias of a neuron at a specified index
 * @param neuronIndex: The index of the neuron
 * @param bias: The new bias of the neuron
*/
void Layer:: setNeuronBias(int neuronIndex, float bias){
    this->biases[neuronIndex] = bias;
}

vector<vector<float>> Layer::getWeightDerivatives()
{
    return weightDerivatives;
}

vector<float> Layer::getBiasDerivatives()
{
    return biasDerivatives;
}

/**
 * Returns a string representation of the layer
 * @return: A string representation of the layer
*/
string Layer::toString(){
    string output = "";
    for(size_t neuronIndex = 0; neuronIndex < num_outputs; neuronIndex++){
        output += "Neuron " + to_string(neuronIndex) + ": " + to_string(biases[neuronIndex]) + " + ";
        for (size_t inputIndex = 0; inputIndex < num_inputs - 1; inputIndex++){
            output += to_string(weights[neuronIndex][inputIndex]) + "x" + to_string(inputIndex) + " + ";

        }
        output += to_string(weights[neuronIndex][num_inputs - 1]) + "x" + to_string(num_inputs - 1);
        output += "\n";
    }
    return output;
}

