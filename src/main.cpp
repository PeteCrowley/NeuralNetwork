#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "VisualizeClassification.h"
#include "Network.h"
#include "EvaluationFunctions.h"


using namespace std;

int main(int argc, char** argv)
{
    // Hyperparameters
    float LEARN_RATE = 0.1;
    int EPOCHS_PER_DECAY = 10;
    int BATCH_SIZE = 32;
    int NUM_POINTS = 10000;
    int SCREEN_SIZE = 500;
    float MAX_POINT_VALUE = 10;
    std::string EVAL_FUNCTION_NAME = "3linear";

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--learn_rate") {
            LEARN_RATE = std::atof(argv[++i]);
        } else if (arg == "--epochs_per_decay") {
            EPOCHS_PER_DECAY = std::atoi(argv[++i]);
        } else if (arg == "--batch_size") {
            BATCH_SIZE = std::atoi(argv[++i]);
        } else if (arg == "--num_points") {
            NUM_POINTS = std::atoi(argv[++i]);
        } else if (arg == "--screen_size") {
            SCREEN_SIZE = std::atoi(argv[++i]);
        } else if (arg == "--max_point_value") {
            MAX_POINT_VALUE = std::atof(argv[++i]);
        } 
        else if (arg == "--eval_function") {
            EVAL_FUNCTION_NAME = argv[++i];
        }
        else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return 1;
        }
    }
    EvalFunctionPtr evalFunction = EvaluationFunctions::function_map.at(EVAL_FUNCTION_NAME);
    int numberOfClasses = EvaluationFunctions::num_classes_map.at(EVAL_FUNCTION_NAME);

    // Create the network
    Layer layerOne(2, 16, tanH);
    Layer layerTwo(16, 16, tanH);
    Layer layerFour(16, numberOfClasses, sigmoid);
    Network network(vector<Layer>{layerOne, layerTwo, layerFour}, LEARN_RATE, EPOCHS_PER_DECAY, BATCH_SIZE);

    // Initialize the Visualizer and run the main loop
    VisualizeClassification visualizer(evalFunction, network, NUM_POINTS, SCREEN_SIZE, MAX_POINT_VALUE);
    visualizer.runMainLoop();

    return 0;
}
