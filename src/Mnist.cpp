#include <stdlib.h>
#include <iostream>
#include <vector>

#include "CSVReader.h"
#include "Network.h"
#include "Layer.h"
#include "ActivationFunction.h"
#include "EvaluationFunctions.h"


static Network* g_network = nullptr;
static vector<vector<float>> g_test_inputs;
static vector<int> g_test_labels;


void networkUpdateFunction(float loss, float epoch_time, float total_time, int epoch){
    float test_accuracy = 0.0f;
    if (g_network != nullptr && !g_test_inputs.empty() && g_test_inputs.size() == g_test_labels.size()) {
        test_accuracy = g_network->accuracy(g_test_inputs, g_test_labels);
    }

    cout << "Epoch " + to_string(epoch)
            + ": loss = " + to_string(loss)
            + ", test_accuracy = " + to_string(test_accuracy)
            + ", time for epoch = " + to_string(epoch_time)
            + ", total time = " + to_string(total_time)
            << endl;
}


int main(int argc, char** argv){
    // Hyperparameters
    float LEARN_RATE = 0.1;
    int EPOCHS_PER_DECAY = 5;
    int BATCH_SIZE = 64;


    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--learn_rate") {
            LEARN_RATE = std::atof(argv[++i]);
        } else if (arg == "--epochs_per_decay") {
            EPOCHS_PER_DECAY = std::atoi(argv[++i]);
        } else if (arg == "--batch_size") {
            BATCH_SIZE = std::atoi(argv[++i]);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }

    int numberOfOutputs = 10;
    int numberOfInputs = 784;

    // Load the data
    vector<int> training_labels = readIntColumn("datasets/mnist_train.csv", 0, 1, 30001);
    vector<vector<int>> training_data = readIntRows("datasets/mnist_train.csv", 1, 784, 1, 30001);
    vector<vector<float>> normalized_data = normalize(training_data, 255);

    vector<int> test_labels = readIntColumn("datasets/mnist_test.csv", 0, 1, 10000);
    vector<vector<int>> test_data = readIntRows("datasets/mnist_test.csv", 1, 784, 1, 10000);
    vector<vector<float>> normalized_test_data = normalize(test_data, 255);

    g_test_labels = test_labels;
    g_test_inputs = normalized_test_data;

    // Create the network
    Layer layerOne(numberOfInputs, 32, relu);
    Layer layerTwo(32, 32, relu);
    Layer layerThree(32, 16, relu);
    Layer layerFour(16, numberOfOutputs, softmax);
    Network network(vector<Layer>{layerOne, layerTwo, layerThree, layerFour}, LEARN_RATE, EPOCHS_PER_DECAY, BATCH_SIZE, crossEntropy);
    g_network = &network;
    
    // network.train(normalized_data, vectorizeOutputs(training_labels, numberOfOutputs), 5, networkUpdateFunction);
    // network.saveNetwork("Mnist.network");
    network.loadNetwork("Mnist.network");
    cout << "Final Test Accuracy: " + to_string(network.accuracy(normalized_test_data, test_labels)) << endl;
    return 0;
}