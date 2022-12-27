# Histogram of Oriented Gradients (HOG)
## Basic Information
The histogram of oriented gradients (HOG) is a feature descriptor used in computer vision and image processing for the purpose of object detection.

Github repository: [https://github.com/Vinci881027/HOG](https://github.com/Vinci881027/HOG)

## Problem to Solve
There are a lot of methods to do object detection, but HOG is one of the method providing excellent performance.

The principle behind the HOG descriptor is that local object appearance and shape within an image can be described by the distribution of intensity gradients or edge directions. The x and y derivatives of an image (Gradients) are useful because the magnitude of gradients is large around edges and corners due to abrupt change in intensity and we know that edges and corners pack in a lot more information about object shape than flat regions.

In this project, I will implement HOG by C++. Here is the pseudo code:
```
img = imread();

// Calculate gradient gx, gy
Mat gx, gy;
gradient(img, gx);
gradient(img, gy);

// Calculate gradient magnitude and direction (in degrees)
magnitude(gx, gy)
```
Then we calculate the histogram of gradients. Here is how the gradients look.
![](https://learnopencv.com/wp-content/uploads/2016/12/hog-cell-gradients.png)

## Prospective Users
The HOG descriptors focused on the structure or the shape of an object. This method can be used in pedestrian detection in static images, as well as to a variety of common animals and vehicles detection, or even human detection in videos.

## System Architecture
1. Preprocessing
2. Calculate the Gradient Images
3. Calculate Histogram of Gradients in 8x8 cells
4. 16x16 Block Normalization
5. Calculate the Histogram of Oriented Gradients feature vector

## API Description

## Engineering Infrastructure
1. Automatic build system: `make`
2. Version control: `git`
3. Testing framework: `pytest`
4. Documentation: `README.md`

## Schedule
* Planning phase (6 weeks from 9/19 to 10/31): Study domain knowledge
* Week 1 (10/31): Setup C++ and Python environment
* Week 2 (11/7): Image preprocessing 
* Week 3 (11/14): Implement the gradient of image
* Week 4 (11/21): Implement HOG in 8x8 and block normalization in 16x16
* Week 5 (11/28): Implement the HOG feature vector
* Week 6 (12/5): Visualize HOG
* Week 7 (12/12): Testing & Make slides for presentation
* Week 8 (12/19): Project Presentation

## References
[Histograms of Oriented Gradients for Human Detection](https://lear.inrialpes.fr/people/triggs/pubs/Dalal-cvpr05.pdf)

[HOG (Histogram of Oriented Gradients): An Overview](https://towardsdatascience.com/hog-histogram-of-oriented-gradients-67ecd887675f)

[Histogram of Oriented Gradients explained using OpenCV](https://learnopencv.com/histogram-of-oriented-gradients/)
