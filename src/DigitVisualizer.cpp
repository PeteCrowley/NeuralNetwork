#include "DigitVisualizer.h"

#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ActivationFunction.h"
#include "CSVReader.h"
#include "Layer.h"

using namespace std;

DigitVisualizer::DigitVisualizer(Network& network,
                                 const vector<vector<int>>& pixels,
                                 const vector<int>& labels,
                                 int startIndex,
                                 int pixelScale,
                                 int panelWidth)
    : network(network),
      pixels(pixels),
      labels(labels),
      currentIndex(0),
      pixelScale(pixelScale),
      window(sf::VideoMode(sf::Vector2u(kImageSize * pixelScale + panelWidth, kImageSize * pixelScale)),
             "MNIST Digit Visualizer") {
    normalizedPixels.reserve(this->pixels.size());
    for (const vector<int>& image : this->pixels) {
        normalizedPixels.push_back(flattenAndNormalize(image));
    }

    if (!font.openFromFile("fonts/calibri.ttf")) {
        cerr << "Warning: could not load fonts/calibri.ttf; text may not render correctly." << endl;
    }

    if (!this->pixels.empty()) {
        if (startIndex < 0) {
            currentIndex = 0;
        } else if (static_cast<size_t>(startIndex) >= this->pixels.size()) {
            currentIndex = this->pixels.size() - 1;
        } else {
            currentIndex = static_cast<size_t>(startIndex);
        }
    }
}

vector<float> DigitVisualizer::flattenAndNormalize(const vector<int>& image) const {
    vector<float> normalized;
    normalized.reserve(image.size());
    for (int value : image) {
        normalized.push_back(static_cast<float>(value) / 255.0f);
    }
    return normalized;
}

void DigitVisualizer::moveIndex(int delta) {
    if (pixels.empty()) {
        return;
    }

    const int size = static_cast<int>(pixels.size());
    int newIndex = static_cast<int>(currentIndex) + delta;

    while (newIndex < 0) {
        newIndex += size;
    }
    while (newIndex >= size) {
        newIndex -= size;
    }

    currentIndex = static_cast<size_t>(newIndex);
}

void DigitVisualizer::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            continue;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Right || keyPressed->code == sf::Keyboard::Key::Down) {
                moveIndex(1);
            } else if (keyPressed->code == sf::Keyboard::Key::Left || keyPressed->code == sf::Keyboard::Key::Up) {
                moveIndex(-1);
            } else if (keyPressed->code == sf::Keyboard::Key::Space) {
                moveIndex(1);
            } else if (keyPressed->code == sf::Keyboard::Key::Escape) {
                window.close();
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                moveIndex(1);
            } else if (mousePressed->button == sf::Mouse::Button::Right) {
                moveIndex(-1);
            }
        }
    }
}

void DigitVisualizer::drawCurrentDigit() {
    window.clear(sf::Color(24, 24, 28));

    if (pixels.empty()) {
        sf::Text noData(font, "No images loaded.", 22);
        noData.setFillColor(sf::Color::White);
        noData.setPosition({20.f, 20.f});
        window.draw(noData);
        window.display();
        return;
    }

    sf::Image image;
    image.resize(sf::Vector2u(kImageSize, kImageSize), sf::Color::Black);
    const vector<int>& digit = pixels[currentIndex];

    for (int y = 0; y < kImageSize; y++) {
        for (int x = 0; x < kImageSize; x++) {
            const int idx = y * kImageSize + x;
            int value = 0;
            if (idx < static_cast<int>(digit.size())) {
                value = std::clamp(digit[idx], 0, 255);
            }
            image.setPixel(sf::Vector2u(static_cast<unsigned int>(x), static_cast<unsigned int>(y)),
                           sf::Color(static_cast<uint8_t>(value), static_cast<uint8_t>(value), static_cast<uint8_t>(value)));
        }
    }

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        return;
    }

    sf::Sprite sprite(texture);
    sprite.setScale({static_cast<float>(pixelScale), static_cast<float>(pixelScale)});
    window.draw(sprite);

    const vector<float> probabilities = network.getOutput(normalizedPixels[currentIndex]);
    vector<pair<int, float>> rankedPredictions;
    rankedPredictions.reserve(probabilities.size());
    for (size_t i = 0; i < probabilities.size(); i++) {
        rankedPredictions.push_back({static_cast<int>(i), probabilities[i]});
    }
    sort(rankedPredictions.begin(), rankedPredictions.end(), [](const pair<int, float>& a, const pair<int, float>& b) {
        return a.second > b.second;
    });

    const int prediction = rankedPredictions.empty() ? -1 : rankedPredictions.front().first;
    const int label = currentIndex < labels.size() ? labels[currentIndex] : -1;
    const bool correct = prediction == label;

    const size_t topK = std::min(static_cast<size_t>(3), rankedPredictions.size());
    std::ostringstream topPredictionsText;
    topPredictionsText << std::fixed << std::setprecision(2);
    for (size_t i = 0; i < topK; i++) {
        topPredictionsText << "\n" << (i + 1) << ". " << rankedPredictions[i].first
                           << " (" << rankedPredictions[i].second * 100.0f << "%)";
    }

    const float textX = static_cast<float>(kImageSize * pixelScale + 20);
    sf::Text header(font, "MNIST Test Viewer", 24);
    header.setFillColor(sf::Color(230, 230, 255));
    header.setPosition({textX, 20.f});
    window.draw(header);

    sf::Text info(font,
                  "Index: " + to_string(currentIndex) + " / " + to_string(pixels.size() - 1) +
                      "\nTrue Label: " + to_string(label) +
                      "\nPrediction: " + to_string(prediction) +
                      "\nCorrect: " + string(correct ? "Yes" : "No") +
                      "\n\nTop Predictions:" + topPredictionsText.str() +
                      "\n\nControls"
                      "\n- Left click / Right Arrow: Next"
                      "\n- Right click / Left Arrow: Prev"
                      "\n- ESC: Quit",
                  18);
    info.setFillColor(correct ? sf::Color(165, 255, 165) : sf::Color(255, 180, 180));
    info.setPosition({textX, 70.f});
    window.draw(info);

    window.display();
}

void DigitVisualizer::run() {
    while (window.isOpen()) {
        handleEvents();
        drawCurrentDigit();
    }
}

int main(int argc, char** argv) {
    string weightsPath = "Mnist.network";
    int startIndex = 0;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--weights" && i + 1 < argc) {
            weightsPath = argv[++i];
        } else if (arg == "--start" && i + 1 < argc) {
            startIndex = std::atoi(argv[++i]);
        } else {
            cerr << "Unknown argument: " << arg << endl;
            cerr << "Usage: ./DigitVisualizer [--weights <file>] [--start <index>]" << endl;
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

    vector<int> testLabels = readIntColumn("datasets/mnist_test.csv", 0, 1, 10000);
    vector<vector<int>> testPixels = readIntRows("datasets/mnist_test.csv", 1, 784, 1, 10000);

    if (testPixels.empty() || testLabels.empty()) {
        cerr << "No MNIST test data loaded." << endl;
        return 1;
    }
    if (testPixels.size() != testLabels.size()) {
        cerr << "Data size mismatch: " << testPixels.size() << " images, " << testLabels.size() << " labels." << endl;
        return 1;
    }

    DigitVisualizer visualizer(network, testPixels, testLabels, startIndex);
    visualizer.run();
    return 0;
}