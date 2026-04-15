 #ifndef DIGIT_VISUALIZER_H
 #define DIGIT_VISUALIZER_H

 #include <SFML/Graphics.hpp>
 #include <vector>
 #include <string>

 #include "Network.h"

 class DigitVisualizer {
 private:
	 static constexpr int kImageSize = 28;

	 Network& network;
	 std::vector<std::vector<int>> pixels;
	 std::vector<std::vector<float>> normalizedPixels;
	 std::vector<int> labels;

	 std::size_t currentIndex;
	 int pixelScale;

	 sf::RenderWindow window;
	 sf::Font font;

	 std::vector<float> flattenAndNormalize(const std::vector<int>& image) const;
	 void moveIndex(int delta);
	 void handleEvents();
	 void drawCurrentDigit();

 public:
	 DigitVisualizer(Network& network,
					 const std::vector<std::vector<int>>& pixels,
					 const std::vector<int>& labels,
					 int startIndex = 0,
					 int pixelScale = 16,
					 int panelWidth = 320);

	 void run();
 };

 #endif // DIGIT_VISUALIZER_H
