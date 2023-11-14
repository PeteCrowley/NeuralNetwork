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

int main()
{
    
    Layer layerOne(2, 4, relu);
    Layer layerTwo(4 , 4, tanH);
    Layer layerThree(4, 3, sigmoid);
    Network network(2, 3, vector<Layer>{layerOne, layerTwo, layerThree}, 0.1, 32);
    VisualizeClassification visualizer(three_point_eval_function, network, 5000, 500, 10);
    visualizer.runMainLoop();

    return 0;
}
