# Neural Network
This project is a neural network coded from scratch in C++. It visualizes the learning process of a classifier by generating `num_points` points and classifying them best on predefined evaluation functions (i.e 3 linear sections, 4 squares or 4 concentric circles). Then, each epoch, as the network learns, it colors the screen based on which pixels belong to which class. See the examples below for more detail. Additionally, I tested my project on the Mnist dataset of digits and even a relatively small network achieved ~97% accuracy.

## UI Tools
This project now includes a few small SFML user interfaces for exploring model behavior.

1. `VisualizeClassification`
- Shows the model learning a 2D decision boundary in real time.
- Colors the full screen based on the predicted class and overlays training points.

2. `DigitVisualizer`
- Lets you step through MNIST test images and compare true label vs prediction.
- Displays top prediction options with confidence values.
- Controls:
	- Left click / Right Arrow: next image
	- Right click / Left Arrow: previous image
	- ESC: quit

3. `DigitDrawer`
- Provides a 28x28 drawing canvas where you can sketch a digit.
- Click **Predict** to run inference with the loaded MNIST network.
- Shows top predictions with confidence values.
- Controls:
	- Left click + drag: draw
	- Predict button or `P` / `Enter`: run prediction
	- Clear button or `C`: clear canvas
	- ESC: quit

## Running the UI
Build all targets:

```bash
make
```

Run specific tools:

```bash
./VisualizeClassification --eval_function 4circles
./DigitVisualizer --weights Mnist.network
./DigitDrawer --weights Mnist.network
```

## Required Packages
1. SFML
2. OpenMP

## Examples
https://github.com/user-attachments/assets/5c82bc6d-efb3-4617-b40c-f4792ee6d0a8

https://github.com/user-attachments/assets/3b86569e-a184-40c0-a4be-9fcdd8cb9a7e

https://github.com/user-attachments/assets/1199349b-af97-431b-9a5e-46ec98cd76a9

