#ifndef VISUALIZE_CLASSIFICATION_H
#define VISUALIZE_CLASSIFICATION_H

#include <SFML/Graphics.hpp>
#include "Network.h"
#include <chrono>

/**
 * This is a class used to visualize the process of neural networks learning to classify points
*/
class VisualizeClassification {
private:
    int num_points;             // number of data points
    std::vector<std::vector<float>> inputs;                // input values of data points
    std::vector<int> outputs;                               // outputs of data points from eval function
    std::vector<std::vector<float>> vectorizedOutputs;     // outputs of data points from eval function
    float max_value;      // maximum value of x and y
    // Colors for each output
    const sf::Color colors[8] = {sf::Color::Green, sf::Color::Red, 
                                sf::Color::Blue, sf::Color::Yellow, 
                                sf::Color::Magenta, sf::Color::Cyan, 
                                sf::Color::White, sf::Color::Black};
    sf::Color transparentColors[8];     // transparent versions of the colors

    float screen_size_x;       // size of the screen in x direction
    float screen_size_y;       // size of the screen in y direction
    sf::RenderWindow window;    // window to draw to
    int (*eval_function)(float, float);   // function to generate outputs
    Network network;   // network to generate outputs
    std::chrono::time_point<std::chrono::system_clock> start_time;  // time when the visualization started

public:
    VisualizeClassification(int (*eval_function)(float, float), Network network);
    VisualizeClassification(int (*eval_function)(float, float), Network network, int num_points, int screen_size, float max_value = 10);
    void initializeTransparentColors();
    void generateRandomData();
    void generateOutputs();
    void classifyPointsTransparently();
    void drawPoints();
    void runMainLoop();
    void showNetworkInfo();
};

#endif // VISUALIZE_CLASSIFICATION_H
