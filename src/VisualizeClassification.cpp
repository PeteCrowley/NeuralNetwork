#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <algorithm>
#include <random>
#include <chrono>

#include "Network.h"
#include "VisualizeClassification.h"
#include "Layer.h"
#include "EvaluationFunctions.h"



VisualizeClassification::VisualizeClassification(int (*eval_function)(float, float), Network network) : VisualizeClassification(eval_function, network, 1000, 300) {
}   

/**
 * Constructor for class VisualizeClassification
 * @param eval_function: function to cassify points
 * @param network: network learning classify points
 * @param num_points: number of data points to create and classify
 * @param screen_size: size of the screen to draw to
 * @param max_value: maximum value of x and y for data points
 * 
*/
VisualizeClassification::VisualizeClassification(int (*eval_function)(float, float), Network network, int num_points, int screen_size, float max_value)
    : window(sf::VideoMode(screen_size, screen_size), "Classification Visualization") {
    this->num_points = num_points;
    this->eval_function = eval_function;
    this->network = network;
    this->max_value = max_value;
    generateRandomData();
    generateOutputs();

    screen_size_x = window.getSize().x;
    screen_size_y = window.getSize().y;
    VisualizeClassification::initializeTransparentColors();
}

/**
 * Initializes the transparent colors to be used later
*/
void VisualizeClassification:: initializeTransparentColors(){
    for(int i = 0; i < 8; i++){
        transparentColors[i] = sf::Color(colors[i].r, colors[i].g, colors[i].b, 100);
    }
}

/**
 * Runs the main loop of the visualization
*/
void VisualizeClassification::runMainLoop(){
    // network.train(inputs, vectorizedOutputs, 100);
    window.clear();
    VisualizeClassification::classifyPointsTransparently();
    VisualizeClassification::drawPoints();
    showNetworkInfo();
    window.display();

    start_time = std::chrono::system_clock::now();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
                
        }
        window.clear();
        network.learnWithBatchSize(inputs, vectorizedOutputs);
        VisualizeClassification::classifyPointsTransparently();
        VisualizeClassification::drawPoints();
        VisualizeClassification::showNetworkInfo();
        window.display();
    }
}

/**
 * Generates random data points for the class
*/
void VisualizeClassification::generateRandomData(){
    for(int i = 0; i < num_points; i++){
        inputs.push_back(std::vector<float>{(float)rand() / RAND_MAX * max_value, (float)rand() / RAND_MAX * max_value});
    }
}

/**
 * Shows the loss and number of epochs on the screen
*/
void VisualizeClassification::showNetworkInfo(){
    sf::Font font;
    if(!font.loadFromFile("fonts/calibri.ttf")){
        cout << "Font not loaded" << endl;
        return;
    }

    auto currentTime = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = currentTime - start_time;
    float secondsCount = elapsed_seconds.count();

    int numEpochs = network.getEpoch();
    if (numEpochs == 0){
        secondsCount = 0;
    }
    
    sf::Text loss;
    loss.setFont(font);
    loss.setString("Epoch: " + std::to_string(numEpochs)
                    + "\nLoss: " + std::to_string(network.averageCost(network.getOutputs(inputs), vectorizedOutputs))
                    + "\nAccuracy: " + std::to_string(network.accuracy(inputs, outputs))
                    + "\nTime: " + std::to_string(secondsCount) + " sec");
    loss.setCharacterSize(15);
    loss.setFillColor(sf::Color::White);
    loss.setPosition(10, 10);
    window.draw(loss);


}

/**
 * Generates the outputs for the data points
*/
void VisualizeClassification::generateOutputs(){
    for(int i = 0; i < num_points; i++){
        int output = this->eval_function(inputs[i][0], inputs[i][1]);
        outputs.push_back(output);
        vectorizedOutputs.push_back(network.intToVector(output));
    }
}

/**
 * Classifies the points with a transparent mask over the screen
*/
void VisualizeClassification::classifyPointsTransparently(){
    sf::Image graph;
    graph.create(screen_size_x, screen_size_y, sf::Color::Transparent);
    // Parallelize the nested loops
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int x = 1; x < screen_size_x; x++){
        for (int y = 1; y < screen_size_y; y++){
            float scaled_x = x / screen_size_x * max_value;
            float scaled_y = (screen_size_y - y) / screen_size_y * max_value;
            int prediction = network.classify(std::vector<float>{scaled_x, scaled_y});
            graph.setPixel(x, y, transparentColors[prediction]);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(graph);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
}

/**
 * Draws the points to the screen
*/
void VisualizeClassification::drawPoints(){
    for (int i = 0; i < num_points; i++){
        sf::CircleShape point(1);
        point.setPosition(inputs[i][0] * screen_size_x / max_value, screen_size_y - inputs[i][1] * screen_size_y / max_value);
        point.setFillColor(colors[outputs[i]]);
        window.draw(point);
    }
}

/**
 * Main function
*/
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
            if (EvaluationFunctions::function_map.find(EVAL_FUNCTION_NAME) == EvaluationFunctions::function_map.end()) {
                std::cerr << "Unknown evaluation function: " << EVAL_FUNCTION_NAME << "\nCheck EvaluationFunctions.cpp for a list of available functions, or add you own custom function" << std::endl;
                return 1;
            }
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
    Layer layerThree(16, numberOfClasses, sigmoid);
    Network network(vector<Layer>{layerOne, layerTwo, layerThree}, LEARN_RATE, EPOCHS_PER_DECAY, BATCH_SIZE);

    // Initialize the Visualizer and run the main loop
    VisualizeClassification visualizer(evalFunction, network, NUM_POINTS, SCREEN_SIZE, MAX_POINT_VALUE);
    visualizer.runMainLoop();

    return 0;
}