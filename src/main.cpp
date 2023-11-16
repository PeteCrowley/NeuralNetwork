#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "VisualizeClassification.h"
#include "Network.h"
#include "EvaluationFunctions.h"

using namespace std;

int main()
{
    // Hyperparameters
    double LEARN_RATE = 0.1;
    int EPOCHS_PER_DECAY = 10;
    int BATCH_SIZE = 32;
    int NUM_POINTS = 10000;
    int SCREEN_SIZE = 500;
    double MAX_POINT_VALUE = 10;
    int (*EVAL_FUNCTION)(double, double) = EvaluationFunctions::four_class_circle;
    int NUM_CLASSES = 4;
    
    // Create the network
    Layer layerOne(2, 16, tanH);
    Layer layerTwo(16, 8, tanH);
    Layer layerFour(8, NUM_CLASSES, sigmoid);
    Network network(vector<Layer>{layerOne, layerTwo, layerFour}, LEARN_RATE, EPOCHS_PER_DECAY, BATCH_SIZE);

    // Initialize the Visualizer and run the main loop
    VisualizeClassification visualizer(EVAL_FUNCTION, network, NUM_POINTS, SCREEN_SIZE, MAX_POINT_VALUE);
    visualizer.runMainLoop();

    return 0;
}
