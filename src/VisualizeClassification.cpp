#include <SFML/Graphics.hpp>
#include "VisualizeClassification.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Network.h"


VisualizeClassification::VisualizeClassification(int (*eval_function)(double, double), Network network) : VisualizeClassification(eval_function, network, 1000, 300) {
}   

VisualizeClassification::VisualizeClassification(int (*eval_function)(double, double), Network network, int num_points, int screen_size, double max_value)
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

void VisualizeClassification:: initializeTransparentColors(){
    for(int i = 0; i < 8; i++){
        transparentColors[i] = sf::Color(colors[i].r, colors[i].g, colors[i].b, 100);
    }
}

void VisualizeClassification::runMainLoop(){
    window.clear();
    VisualizeClassification::classifyPointsTransparently();
    VisualizeClassification::drawPoints();
    showLossAndEpochs();
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
                
        }
        window.clear();
        network.learnWithBatchSize(inputs, vectorizedOutputs, 16);
        VisualizeClassification::classifyPointsTransparently();
        VisualizeClassification::drawPoints();
        VisualizeClassification::showLossAndEpochs();
        window.display();
        
        // VisualizeClassification::visualizeBatchLearning();
    }
}

void VisualizeClassification::generateRandomData(){
    for(int i = 0; i < num_points; i++){
        inputs.push_back(std::vector<double>{(double)rand() / RAND_MAX * max_value, (double)rand() / RAND_MAX * max_value});
    }
}

void VisualizeClassification::showLossAndEpochs(){
    sf::Font font;
    if(!font.loadFromFile("../fonts/calibri.ttf")){
        cout << "Font not loaded" << endl;
        return;
    }

    
    sf::Text loss;
    loss.setFont(font);
    loss.setString("Loss: " + std::to_string(network.averageCost(network.getOutputs(inputs), vectorizedOutputs))
                    + "\nEpoch: " + std::to_string(network.getEpoch()));
    loss.setCharacterSize(15);
    loss.setFillColor(sf::Color::White);
    loss.setPosition(10, 10);
    window.draw(loss);


}

void VisualizeClassification::visualizeBatchLearning(){
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

    for(int i = 0; i < inputs.size() - network.getBatchSize(); i += network.getBatchSize()){
        vector<vector<double>> batch_inputs;
        vector<vector<double>> batch_outputs;
        for(int j = 0; j < network.getBatchSize(); j++){
            batch_inputs.push_back(inputs[indexes[i + j]]);
            batch_outputs.push_back(vectorizedOutputs[indexes[i + j]]);
        }
        network.learn(batch_inputs, batch_outputs);
        window.clear();
        VisualizeClassification::classifyPointsTransparently();
        VisualizeClassification::drawPoints();
        window.display();
    }
}

void VisualizeClassification::generateOutputs(){
    for(int i = 0; i < num_points; i++){
        int output = this->eval_function(inputs[i][0], inputs[i][1]);
        outputs.push_back(output);
        vectorizedOutputs.push_back(network.intToVector(output));
    }
}

void VisualizeClassification::classifyPointsTransparently(){
    sf::Image graph;
    graph.create(screen_size_x, screen_size_y, sf::Color::Transparent);
    for (int x = 1; x < screen_size_x; x++){
        for (int y = 1; y < screen_size_y; y++){
            double scaled_x = x / screen_size_x * max_value;
            double scaled_y = (screen_size_y - y) / screen_size_y * max_value;
            graph.setPixel(x, y, transparentColors[network.classify(std::vector<double>{scaled_x, scaled_y})]);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(graph);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
}

void VisualizeClassification::drawPoints(){
    for (int i = 0; i < num_points; i++){
        sf::CircleShape point(1);
        point.setPosition(inputs[i][0] * screen_size_x / max_value, screen_size_y - inputs[i][1] * screen_size_y / max_value);
        point.setFillColor(colors[outputs[i]]);
        window.draw(point);
    }
}