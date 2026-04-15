#include "DigitDrawer.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ActivationFunction.h"
#include "Layer.h"

using namespace std;

DigitDrawer::DigitDrawer(Network& network, int pixelScale, int panelWidth, int brushRadius)
	: network(network),
	  pixelScale(pixelScale),
	  brushRadius(brushRadius),
	  window(sf::VideoMode(sf::Vector2u(kImageSize * pixelScale + panelWidth, kImageSize * pixelScale)), "MNIST Digit Drawer"),
	canvasRect({0.0f, 0.0f}, {static_cast<float>(kImageSize * pixelScale), static_cast<float>(kImageSize * pixelScale)}),
	predictButtonRect({static_cast<float>(kImageSize * pixelScale + 20), 70.0f}, {static_cast<float>(panelWidth - 40), 50.0f}),
	clearButtonRect({static_cast<float>(kImageSize * pixelScale + 20), 135.0f}, {static_cast<float>(panelWidth - 40), 50.0f}),
	  canvasPixels(kImageSize * kImageSize, 0.0f) {
	if (!font.openFromFile("fonts/calibri.ttf")) {
		cerr << "Warning: could not load fonts/calibri.ttf; text may not render correctly." << endl;
	}
}

void DigitDrawer::clearCanvas() {
	std::fill(canvasPixels.begin(), canvasPixels.end(), 0.0f);
	lastPredictions.clear();
}

void DigitDrawer::paintAtWindowPosition(sf::Vector2i windowPos) {
	if (!canvasRect.contains(sf::Vector2f(static_cast<float>(windowPos.x), static_cast<float>(windowPos.y)))) {
		return;
	}

	const int centerX = windowPos.x / pixelScale;
	const int centerY = windowPos.y / pixelScale;

	for (int dy = -brushRadius; dy <= brushRadius; dy++) {
		for (int dx = -brushRadius; dx <= brushRadius; dx++) {
			const int x = centerX + dx;
			const int y = centerY + dy;
			if (x < 0 || x >= kImageSize || y < 0 || y >= kImageSize) {
				continue;
			}

			const float distance = std::sqrt(static_cast<float>(dx * dx + dy * dy));
			const float influence = std::max(0.0f, 1.0f - distance / static_cast<float>(brushRadius + 1));
			const int idx = y * kImageSize + x;
			canvasPixels[idx] = std::min(1.0f, canvasPixels[idx] + 0.55f * influence);
		}
	}
}

void DigitDrawer::runPrediction() {
	const vector<float> probabilities = network.getOutput(canvasPixels);
	lastPredictions.clear();
	lastPredictions.reserve(probabilities.size());

	for (size_t i = 0; i < probabilities.size(); i++) {
		lastPredictions.push_back({static_cast<int>(i), probabilities[i]});
	}

	sort(lastPredictions.begin(), lastPredictions.end(), [](const pair<int, float>& a, const pair<int, float>& b) {
		return a.second > b.second;
	});
}

void DigitDrawer::handleEvents() {
	while (const auto event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
			continue;
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code == sf::Keyboard::Key::Escape) {
				window.close();
			} else if (keyPressed->code == sf::Keyboard::Key::C) {
				clearCanvas();
			} else if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::P) {
				runPrediction();
			}
		}

		if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			const sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));

			if (mousePressed->button == sf::Mouse::Button::Left) {
				if (predictButtonRect.contains(mousePos)) {
					runPrediction();
				} else if (clearButtonRect.contains(mousePos)) {
					clearCanvas();
				} else {
					paintAtWindowPosition(mousePressed->position);
				}
			}
		}

		if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				paintAtWindowPosition(mouseMoved->position);
			}
		}
	}
}

void DigitDrawer::drawCanvas() {
	sf::RectangleShape bg(sf::Vector2f(canvasRect.size.x, canvasRect.size.y));
	bg.setPosition(canvasRect.position);
	bg.setFillColor(sf::Color(16, 16, 18));
	window.draw(bg);

	for (int y = 0; y < kImageSize; y++) {
		for (int x = 0; x < kImageSize; x++) {
			const int idx = y * kImageSize + x;
			const int value = static_cast<int>(std::clamp(canvasPixels[idx], 0.0f, 1.0f) * 255.0f);

			sf::RectangleShape cell(sf::Vector2f(static_cast<float>(pixelScale), static_cast<float>(pixelScale)));
			cell.setPosition({static_cast<float>(x * pixelScale), static_cast<float>(y * pixelScale)});
			cell.setFillColor(sf::Color(static_cast<uint8_t>(value), static_cast<uint8_t>(value), static_cast<uint8_t>(value)));
			window.draw(cell);
		}
	}

	sf::RectangleShape border(sf::Vector2f(canvasRect.size.x - 2.0f, canvasRect.size.y - 2.0f));
	border.setPosition({1.0f, 1.0f});
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color(80, 80, 90));
	border.setOutlineThickness(1.5f);
	window.draw(border);
}

void DigitDrawer::drawButtons() {
	sf::RectangleShape predictButton(sf::Vector2f(predictButtonRect.size.x, predictButtonRect.size.y));
	predictButton.setPosition(predictButtonRect.position);
	predictButton.setFillColor(sf::Color(64, 130, 80));
	window.draw(predictButton);

	sf::Text predictLabel(font, "Predict", 24);
	predictLabel.setFillColor(sf::Color::White);
	predictLabel.setPosition({predictButtonRect.position.x + 18.0f, predictButtonRect.position.y + 10.0f});
	window.draw(predictLabel);

	sf::RectangleShape clearButton(sf::Vector2f(clearButtonRect.size.x, clearButtonRect.size.y));
	clearButton.setPosition(clearButtonRect.position);
	clearButton.setFillColor(sf::Color(125, 62, 62));
	window.draw(clearButton);

	sf::Text clearLabel(font, "Clear", 24);
	clearLabel.setFillColor(sf::Color::White);
	clearLabel.setPosition({clearButtonRect.position.x + 18.0f, clearButtonRect.position.y + 10.0f});
	window.draw(clearLabel);
}

void DigitDrawer::drawPredictions() {
	const float panelX = static_cast<float>(kImageSize * pixelScale + 20);

	sf::Text title(font, "Draw a digit", 26);
	title.setFillColor(sf::Color(230, 230, 255));
	title.setPosition({panelX, 20.0f});
	window.draw(title);

	std::ostringstream textBuilder;
	textBuilder << std::fixed << std::setprecision(2);
	textBuilder << "Top Predictions:";

	const size_t topK = std::min(static_cast<size_t>(3), lastPredictions.size());
	if (topK == 0) {
		textBuilder << "\n(click Predict)";
	} else {
		for (size_t i = 0; i < topK; i++) {
			textBuilder << "\n" << (i + 1) << ". " << lastPredictions[i].first
						<< " (" << lastPredictions[i].second * 100.0f << "%)";
		}
	}

	textBuilder << "\n\nControls"
				<< "\n- Drag Left Mouse: Draw"
				<< "\n- Predict button or P/Enter"
				<< "\n- Clear button or C"
				<< "\n- ESC: Quit";

	sf::Text details(font, textBuilder.str(), 18);
	details.setFillColor(sf::Color(230, 230, 230));
	details.setPosition({panelX, 205.0f});
	window.draw(details);
}

void DigitDrawer::drawUI() {
	window.clear(sf::Color(24, 24, 28));
	drawCanvas();
	drawButtons();
	drawPredictions();
	window.display();
}

void DigitDrawer::run() {
	while (window.isOpen()) {
		handleEvents();
		drawUI();
	}
}

int main(int argc, char** argv) {
	string weightsPath = "Mnist.network";

	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "--weights" && i + 1 < argc) {
			weightsPath = argv[++i];
		} else {
			cerr << "Unknown argument: " << arg << endl;
			cerr << "Usage: ./DigitDrawer [--weights <file>]" << endl;
			return 1;
		}
	}

	const int numberOfInputs = 784;
	const int numberOfOutputs = 10;

	Layer layerOne(numberOfInputs, 32, relu);
	Layer layerTwo(32, 32, relu);
	Layer layerThree(32, 16, relu);
	Layer layerFour(16, numberOfOutputs, softmax);
	Network network(vector<Layer>{layerOne, layerTwo, layerThree, layerFour}, 0.1f, 5, 64, crossEntropy);

	if (!network.loadNetwork(weightsPath)) {
		cerr << "Failed to load network weights from: " << weightsPath << endl;
		return 1;
	}

	DigitDrawer drawer(network);
	drawer.run();
	return 0;
}
