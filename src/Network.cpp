#include "Network.h"
#include "Layer.h"
#include "CostFunction.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

/**
 * Default constructor for Network
 */

Network::Network(){
    this->num_inputs = 0;
    this->num_outputs = 0;
    this->num_layers = 0;
}

/**
 * Constructor for class Network
 * @param layer: The layer to add to the network
 */
Network::Network(Layer layer) : Network(vector<Layer>{layer}){}

/**
 * Constructor for class Network
 * @param layers: The layers to add to the network
 * @param learn_rate: The learning rate of the network
 * @param epochs_per_decay: The number of epochs before the learning rate is halved
 * @param batch_size: The batch size for learning
 * @param costFunction: The cost function of the network
*/
Network::Network(vector<Layer> layers, double learn_rate, int epochs_per_decay, int batch_size, CostFunction costFunction){
    this->num_inputs = layers[0].getNumInputs();
    this->num_layers = layers.size();
    this->num_outputs = layers[num_layers-1].getNumOutputs();
    this->layers = layers;
    this->learn_rate = learn_rate;
    this->batch_size = batch_size;
    this->epoch = 0;
    this->epoch_decay_rate = epochs_per_decay;
    this->costFunction = costFunction;

    // Check that the number of outputs for each layer is the same as the number of inputs for the next layer
    for(int i = 0; i < num_layers-1; i++){
        if (layers[i].getNumOutputs() != layers[i+1].getNumInputs()){
            cout << "Error: Invalid number of outputs for layer " + to_string(i) + ": " + to_string(layers[i].getNumOutputs()) + " instead of expected " + to_string(layers[i+1].getNumInputs()) << endl;
            exit(1);
        }
    }
}

/**
 * Gets the output of the network for a given input
 * @param inputs: The inputs to the network
 * @return: The output of the network
*/
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

/**
 * Gets the outputs of the network for a given set of inputs
 * @param inputs: The inputs to the network
 * @return: The outputs of the network
*/
vector<vector<double>> Network:: getOutputs(vector<vector<double>> inputs){
    vector<vector<double>> outputs;
    for(int i = 0; i < inputs.size(); i++){
        outputs.push_back(this->getOutput(inputs[i]));
    }
    return outputs;
}

/**
 * Classifies a given input to the network
 * @param input: The input to the network
 * @return: The integer classification of the inputs
*/
int Network::classify(vector<double> input){
    if (input.size() != num_inputs){
        cout << "Error: Invalid number of inputs: " + to_string(input.size()) + " instead of expected " + to_string(num_inputs) << endl;
        exit(1);
    }
    vector<double> output = this->getOutput(input);

    double max_index = 0;
    for(int i = 0; i < output.size(); i++){
        if (output[i] > output[max_index]){
            max_index = i;
        }
    }
    return max_index;
}

/**
 * Calculates the cost of the network for a given output and expected output
 * @param output: The output of the network
 * @param expected_output: The expected output of the network
 * @return: The cost of the network
*/
double Network::cost(std::vector<double> output, vector<double> expected_output){
    double total_error = 0;
    for(int i = 0; i < output.size(); i++){
        total_error += costFunction.cost(output[i], expected_output[i]);
    }
    return total_error;
}

/**
 * Calculates the average cost of the network for a given set of outputs and expected outputs
 * @param outputs: The outputs of the network
 * @param expected_outputs: The expected outputs of the network
 * @return: The average cost of the network for the given outputs and expected outputs
*/
double Network:: averageCost(vector<vector<double>> outputs, vector<vector<double>> expected_outputs){
    double total_cost = 0;
    for (int i = 0; i < expected_outputs.size(); i++){
        total_cost += this->cost(outputs[i], expected_outputs[i]);
    }
    return total_cost / expected_outputs.size();
}

/**
 * Calculates the accuracy of the network for a given set of inputs and expected outputs
 * @param inputs: The inputs to the network
 * @param expected_integer_outputs: The expected outputs of the network
 * @return: The percentage of data points classified correctly
*/
double Network::accuracy(vector<vector<double>> inputs, vector<int> expected_integer_outputs)
{
    int total_correct = 0;
    for(int i = 0; i < inputs.size(); i++){
        total_correct += this->classify(inputs[i]) == expected_integer_outputs[i];   
    }
    return total_correct / (double)inputs.size(); 
}

/**
 * Given a set of inputs and expected outputs, using backpropogation to adjust the weights and biases of the network
 * @param input: The input to the network
 * @param expected_output: The expected output of the network
*/
void Network::quickLearn(vector<vector<double>> inputs, vector<vector<double>> expected_output)
{
    for(int i = 0; i < inputs.size(); i++){
        this->updateDerivatives(inputs[i], expected_output[i]);
    }

    applyDerivatives(inputs.size());

    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        layers[layerIndex].resetDerivatives();
    }
}

/**
 * Given an input and output, calculate how much to adjust each weight and bias in the network
 * @param input: The input to the network
 * @param expected_output: The expected output of the network
*/
void Network::updateDerivatives(vector<double> input, vector<double> expected_output)
{
    this->getOutput(input);

    // First calculate derivatives for output layer, then work from there
    layers[num_layers - 1].calculateOutputLayerPartialDerivatives(costFunction, expected_output);
    layers[num_layers - 1].calculateBiasPartialDerivatives(layers[num_layers - 1]);

    // Calculate all of the partials for all of the weights and biases in all layers
    for(int layerIndex = num_layers - 2; layerIndex >= 0; layerIndex--){
        layers[layerIndex].calculateHiddenLayerPartialDerivatives(layers[layerIndex + 1]);
        layers[layerIndex].calculateBiasPartialDerivatives(layers[layerIndex + 1]);
    }

}

/**
 * Applies the previously calculated derivatives to the weights and biases of the network
 * @param batch_size: how many points the derivatives are calculated from
*/
void Network::applyDerivatives(int batch_size){
    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        for(int neuronIndex = 0; neuronIndex < layers[layerIndex].getNumOutputs(); neuronIndex++){
            for(int inputIndex = 0; inputIndex < layers[layerIndex].getNumInputs(); inputIndex++){
                double original_weight = layers[layerIndex].getWeight(neuronIndex, inputIndex);
                double derivative = layers[layerIndex].getWeightDerivatives()[neuronIndex][inputIndex];
                layers[layerIndex].setNeuronWeight(neuronIndex, inputIndex, original_weight - learn_rate * derivative / batch_size);
            }
            layers[layerIndex].setNeuronBias(neuronIndex, layers[layerIndex].getBias(neuronIndex) - learn_rate * layers[layerIndex].getBiasDerivatives()[neuronIndex] / batch_size);
        }
    }
}


/**
 * Divide dataa into batches then feed into quickLearn for backpropogation
 * @param inputs: The inputs to the network
 * @param expected_outputs: The expected outputs of the network
*/
void Network::learnWithBatchSize(vector<vector<double>> inputs, vector<vector<double>> expected_outputs)
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
    for(int i = 0; i < inputs.size(); i += batch_size){
        vector<vector<double>> batch_inputs;
        vector<vector<double>> batch_outputs;
        int j = 0;
        while(j < batch_size && i + j < inputs.size()){
            batch_inputs.push_back(inputs[indexes[i + j]]);
            batch_outputs.push_back(expected_outputs[indexes[i + j]]);
            j++;
        }
        this->quickLearn(batch_inputs, batch_outputs);
    }
    
    
    if((epoch % epoch_decay_rate) == 0){
        learn_rate *= 0.5;
    }

}


/**
 * Converts an integer to a vector of length num_outputs with a 1 in the index of the integer
*/
std::vector<double> Network::intToVector(int num)
{
    std::vector<double> ret;
    for(int i = 0; i < num_outputs; i++){
        ret.push_back(i == num ? 1 : 0);
    }
    return ret;
}

/**
 * Gets the batch size of the network
 * @return: The batch size of the network
*/
int Network::getEpoch()
{
    return epoch;
}

/**
 * Gets the string representation of the network
 * @return: a string representation of the network
*/
string Network::toString(){
    string output = "";
    for(int layerIndex = 0; layerIndex < num_layers; layerIndex++){
        output += "Layer " + to_string(layerIndex+1) + ":\n" + layers[layerIndex].toString() + "\n";
    }
    return output;
}



