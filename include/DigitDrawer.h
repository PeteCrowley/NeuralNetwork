#ifndef DIGIT_DRAWER_H
#define DIGIT_DRAWER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Network.h"

class DigitDrawer {
private:
	static constexpr int kImageSize = 28;

	Network& network;
	int pixelScale;
	int brushRadius;

	sf::RenderWindow window;
	sf::Font font;

	sf::FloatRect canvasRect;
	sf::FloatRect predictButtonRect;
	sf::FloatRect clearButtonRect;

	std::vector<float> canvasPixels;
	std::vector<std::pair<int, float>> lastPredictions;

	void clearCanvas();
	void paintAtWindowPosition(sf::Vector2i windowPos);
	void runPrediction();

	void handleEvents();
	void drawUI();
	void drawCanvas();
	void drawButtons();
	void drawPredictions();

public:
	DigitDrawer(Network& network, int pixelScale = 18, int panelWidth = 340, int brushRadius = 1);
	void run();
};

#endif // DIGIT_DRAWER_H
