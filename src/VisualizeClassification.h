#ifndef VISUALIZE_CLASSIFICATION_H
#define VISUALIZE_CLASSIFICATION_H

#include <SFML/Graphics.hpp>
#include "Network.h"

/**
 * This is a class used to visualize the process of neural networks learning to classify points
*/
class VisualizeClassification {
private:
    int num_points;             // number of data points
    std::vector<std::vector<double>> inputs;                // input values of data points
    std::vector<int> outputs;                               // outputs of data points from eval function
    std::vector<std::vector<double>> vectorizedOutputs;     // outputs of data points from eval function
    double max_value;      // maximum value of x and y
    // Colors for each output
    const sf::Color colors[8] = {sf::Color::Green, sf::Color::Red, 
                                sf::Color::Blue, sf::Color::Yellow, 
                                sf::Color::Magenta, sf::Color::Cyan, 
                                sf::Color::White, sf::Color::Black};
    sf::Color transparentColors[8];     // transparent versions of the colors

    double screen_size_x;       // size of the screen in x direction
    double screen_size_y;       // size of the screen in y direction
    sf::RenderWindow window;    // window to draw to
    int (*eval_function)(double, double);   // function to generate outputs
    Network network;   // network to generate outputs

public:
    VisualizeClassification(int (*eval_function)(double, double), Network network);
    VisualizeClassification(int (*eval_function)(double, double), Network network, int num_points, int screen_size, double max_value = 10);
    void initializeTransparentColors();
    void generateRandomData();
    void generateOutputs();
    void classifyPointsTransparently();
    void drawPoints();
    void runMainLoop();
    void showLossAndEpochs();
};

#endif // VISUALIZE_CLASSIFICATION_H
