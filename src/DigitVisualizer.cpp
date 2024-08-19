#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>

int SCREEN_SIZE = 28;

void digitViewerLoop(vector<vector<int>> pixels, vector<int> labels){
    sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE * 10, SCREEN_SIZE * 10), "Digit Viewer");
    
}