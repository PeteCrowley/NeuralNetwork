#include <stdlib.h>
#include <iostream>
#include <vector>

#include "CSVReader.h"
#include "Network.h"
#include "Layer.h"
#include "ActivationFunction.h"
#include "EvaluationFunctions.h"


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
    // EvalFunctionPtr evalFunction = EvaluationFunctions::function_map.at(EVAL_FUNCTION_NAME);
    // int numberOfClasses = EvaluationFunctions::num_classes_map.at(EVAL_FUNCTION_NAME);

    int numberOfOutputs = 10;
    int numberOfInputs = 784;

    // Load the data
    vector<int> training_labels = readIntColumn("datasets/mnist_train.csv", 0, 1, 6001);
    vector<vector<int>> training_data = readIntRows("datasets/mnist_train.csv", 1, 784, 1, 6001);
    vector<vector<float>> normalized_data = normalize(training_data, 255);

    //Print the data
    // for (int i = 0; i < 10; i++){
    //     for (int j = 0; j < 784; j++){
    //         std::cout << training_data[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }


    // Create the network
    Layer layerOne(numberOfInputs, 32, relu);
    Layer layerTwo(32, 32, relu);
    Layer layerThree(32, numberOfOutputs, relu);
    Network network(vector<Layer>{layerOne, layerTwo, layerThree}, LEARN_RATE, EPOCHS_PER_DECAY, BATCH_SIZE);
    network.train(normalized_data, vectorizeOutputs(training_labels, numberOfOutputs), 100);
    return 0;
}