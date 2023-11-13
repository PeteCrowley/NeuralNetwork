#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "VisualizeClassification.h"
#include "Network.h"

using namespace std;

int three_point_eval_function(double x, double y){
    double diff =  x - y;
    if (diff < -1){
        return 0;
    }
    else if (diff < 1){
        return 1;
    }
    else {
        return 2;
    }
}

int four_point_eval_function(double x, double y){
    int pivot = 5;
    x = x - pivot;
    y = y - pivot;
    if (x < 0 && y < 0){
        return 0;
    }
    else if (x < 0 && y > 0){
        return 1;
    }
    else if (x > 0 && y < 0){
        return 2;
    }
    else {
        return 3;
    }
}

int eval_function(double x, double y){
    return x*x < y;
}

int predict_function(vector<double> inputs){
    return 2*inputs[0] < inputs[1];
}

double sigmoid(double x){
    return 1 / (1 + exp(-x));
}

double relu(double x){
    return x > 0 ? x : 0;
}

double tanh(double x){
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

void drawFunction(sf::RenderWindow &window, bool (*function)(double, double), double max_data, int width){
    sf::Image graph;
    graph.create(window.getSize().x, window.getSize().y, sf::Color::Transparent);
    double screen_size_x = window.getSize().x;
    double screen_size_y = window.getSize().y;
    for (int x = 1; x < screen_size_x; x++){
        for (int y = 1; y < screen_size_y; y++){
            double scaled_x = x / screen_size_x * max_data;
            double scaled_y = (screen_size_y - y) / screen_size_y * max_data;
            double adjusted_scaled_y = (screen_size_y - y - width) / screen_size_y * max_data;
            if (function(scaled_x, scaled_y) != function(scaled_x, adjusted_scaled_y)){
                graph.setPixel(x, y, sf::Color::Green);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(graph);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
}

void classifyPointsTransparently(sf::RenderWindow &window, bool (*function)(double, double), double max_data){
    sf::Image graph;
    graph.create(window.getSize().x, window.getSize().y, sf::Color::Transparent);
    double screen_size_x = window.getSize().x;
    double screen_size_y = window.getSize().y;
    sf::Color transparentGreen = sf::Color(0, 255, 0, 100);
    sf::Color transparentRed = sf::Color(255, 0, 0, 100);
    for (int x = 1; x < screen_size_x; x++){
        for (int y = 1; y < screen_size_y; y++){
            double scaled_x = x / screen_size_x * max_data;
            double scaled_y = (screen_size_y - y) / screen_size_y * max_data;
            if (function(scaled_x, scaled_y)){
                graph.setPixel(x, y, transparentGreen);
            } else {
                graph.setPixel(x, y, transparentRed);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(graph);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
}



int main()
{
    
    Layer layerOne(2, 4, relu);
    Layer layerTwo(4 , 4, tanh);
    Layer layerThree(4, 3, sigmoid);
    Network network(2, 3, vector<Layer>{layerOne, layerTwo, layerThree}, 0.1, 32);
    VisualizeClassification visualizer(three_point_eval_function, network, 5000, 500, 10);
    visualizer.runMainLoop();

    return 0;
}
