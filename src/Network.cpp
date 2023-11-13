#include "Network.h"
#include "Layer.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

Network::Network(){
    this->num_inputs = 0;
    this->num_outputs = 0;
    this->num_layers = 0;
}

Network::Network(int num_inputs, int num_outputs, Layer layer) : Network(num_inputs, num_outputs, vector<Layer>{layer}){}

Network::Network(int num_inputs, int num_outputs, vector<Layer> layers, double learn_rate, int batch_size){
    this->num_inputs = num_inputs;
    this->num_outputs = num_outputs;
    this->layers = layers;
    this->num_layers = layers.size();
    this->learn_rate = learn_rate;
    this->batch_size = batch_size;
    this->epoch = 0;
    this->epoch_decay_rate = 10;

    // Check that the number of inputs is correct
    if (layers[0].getNumInputs() != num_inputs){
        cout << "Error: Invalid number of inputs for first layer: " + to_string(layers[0].getNumInputs()) + " instead of expected " + to_string(num_inputs) << endl;
        exit(1);
    }
    // Check the number of outputs for the last layer is correct
    if (layers[num_layers-1].getNumOutputs() != num_outputs){
        cout << "Error: Invalid number of outputs for last layer: " + to_string(layers[num_layers-1].getNumOutputs()) + " instead of expected " + to_string(num_outputs) << endl;
        exit(1);
    }
    // Check that the number of outputs for each layer is the same as the number of inputs for the next layer
    for(int i = 0; i < num_layers-1; i++){
        if (layers[i].getNumOutputs() != layers[i+1].getNumInputs()){
            cout << "Error: Invalid number of outputs for layer " + to_string(i) + ": " + to_string(layers[i].getNumOutputs()) + " instead of expected " + to_string(layers[i+1].getNumInputs()) << endl;
            exit(1);
        }
    }
}

vector<double> Network:: getOutput(vector<double> inputs){
    if (inputs.size() != num_inputs){
        cout << "Error: Invalid number of inputs: " + to_string(inputs.size()) + " instead of expected " + to_string(num_inputs) << endl;
        exit(1);
    }
    vector<double> current_values = inputs;
    for(int i = 0; i < num_layers; i++){
        current_values = layers[i].calculateOutputs(current_values);
    }
    return current_values;
}

vector<vector<double>> Network:: getOutputs(vector<vector<double>> inputs){
    vector<vector<double>> outputs;
    for(int i = 0; i < inputs.size(); i++){
        outputs.push_back(this->getOutput(inputs[i]));
    }
    return outputs;
}

int Network::classify(vector<double> inputs){
    if (inputs.size() != num_inputs){
        cout << "Error: Invalid number of inputs: " + to_string(inputs.size()) + " instead of expected " + to_string(num_inputs) << endl;
        exit(1);
    }
    vector<double> output = this->getOutput(inputs);

    double max_index = 0;
    for(int i = 0; i < output.size(); i++){
        if (output[i] > output[max_index]){
            max_index = i;
        }
    }
    return max_index;
}

double Network::cost(std::vector<double> output, vector<double> expected_output){
    double total_squared_error = 0;
    for(int i = 0; i < output.size(); i++){
        double error = output[i] - expected_output[i];
        total_squared_error += error * error;
    }
    return total_squared_error;
}

double Network:: averageCost(std::vector<std::vector<double>> expected_outputs, vector<vector<double>> outputs){
    double total_cost = 0;
    for (int i = 0; i < expected_outputs.size(); i++){
        total_cost += this->cost(outputs[i], expected_outputs[i]);
    }
    return total_cost / expected_outputs.size();
}

void Network::learn(vector<vector<double>> inputs, vector<vector<double>> outputs){
    const double delta = 0.0001;
    double original_cost = this->averageCost(this->getOutputs(inputs), outputs);

    vector<vector<vector<double>>> weightDerivatives;   // weight derivatives indexed by [layer][neuron][input]
    vector<vector<double>> biasDerivatives;             // weight derivatives indexed by [layer][neuron]


    // Calculate all of the partials for all of the weights and biases in all layers
    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        vector<vector<double>> layer_weight_derivatives;
        vector<double> layer_bias_derivatives;
        for(int neuronIndex = 0; neuronIndex < layers[layerIndex].getNumOutputs(); neuronIndex++){
            vector<double> neuron_weight_derivatives;
            for(int inputIndex = 0; inputIndex < layers[layerIndex].getNumInputs(); inputIndex++){
                double original_weight = layers[layerIndex].getWeight(neuronIndex, inputIndex);
                layers[layerIndex].setNeuronWeight(neuronIndex, inputIndex, original_weight + delta);
                double new_cost = this->averageCost(this->getOutputs(inputs), outputs);
                layers[layerIndex].setNeuronWeight(neuronIndex, inputIndex, original_weight);
                double derivative = (new_cost - original_cost) / delta;
                neuron_weight_derivatives.push_back(derivative);
            }
            // for biases now
            double original_bias = layers[layerIndex].getBias(neuronIndex);
            layers[layerIndex].setNeuronBias(neuronIndex, original_bias + delta);
            double new_cost = this->averageCost(this->getOutputs(inputs), outputs);
            layers[layerIndex].setNeuronBias(neuronIndex, original_bias);

            double derivative = (new_cost - original_cost) / delta;

            layer_weight_derivatives.push_back(neuron_weight_derivatives);
            layer_bias_derivatives.push_back(derivative);
        }
        weightDerivatives.push_back(layer_weight_derivatives);
        biasDerivatives.push_back(layer_bias_derivatives);
    }
    // update everything
    updateBiasesAndWeights(biasDerivatives, weightDerivatives);
}

void Network::updateBiasesAndWeights(std::vector<std::vector<double>> biasDerivatives, vector<vector<vector<double>>> weightDerivatives){
    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        for(int neuronIndex = 0; neuronIndex < layers[layerIndex].getNumOutputs(); neuronIndex++){
            for(int inputIndex = 0; inputIndex < layers[layerIndex].getNumInputs(); inputIndex++){
                double original_weight = layers[layerIndex].getWeight(neuronIndex, inputIndex);
                double derivative = weightDerivatives[layerIndex][neuronIndex][inputIndex];
                layers[layerIndex].setNeuronWeight(neuronIndex, inputIndex, original_weight - learn_rate * derivative);
            }
        }
    }
}

void Network::learnWithBatchSize(vector<vector<double>> inputs, vector<vector<double>> outputs, int batch_size)
{
    epoch++;
    std::vector<int> indexes;
    indexes.reserve(inputs.size());
    for (int i = 0; i < inputs.size(); ++i)
        indexes.push_back(i);
    // Obtain a random seed
    std::random_device rd;
    // Create a random number generator engine
    std::mt19937 rng(rd());
    // Shuffle the vector using std::shuffle
    std::shuffle(indexes.begin(), indexes.end(), rng);
    // print the vectors
    // first inputs

    for(int i = 0; i < inputs.size()-batch_size; i += batch_size){
        vector<vector<double>> batch_inputs;
        vector<vector<double>> batch_outputs;
        for(int j = 0; j < batch_size; j++){
            batch_inputs.push_back(inputs[indexes[i + j]]);
            batch_outputs.push_back(outputs[indexes[i + j]]);
        }
        this->learn(batch_inputs, batch_outputs);
    }
    if((epoch % epoch_decay_rate) == 0){
        learn_rate *= 0.5;
    }

}

std::vector<double> Network::intToVector(int num)
{
    std::vector<double> ret;
    for(int i = 0; i < num_outputs; i++){
        ret.push_back(i == num ? 1 : 0);
    }
    return ret;
}

int Network::getBatchSize()
{
    return batch_size;
}

int Network::getEpoch()
{
    return epoch;
}

string Network::toString(){
    string output = "";
    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        output += "Layer " + to_string(layerIndex+1) + ":\n" + layers[layerIndex].toString() + "\n";
    }
    return output;
}



