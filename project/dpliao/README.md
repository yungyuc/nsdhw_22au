# Histogram Equalization

## Basic Information
Histogram equalization is a method in image processing of contrast adjustment using the image's histogram.
- Github repo link: \
https://github.com/dpliao/histogram-equalization

## Problem to solve
In the field of image processing, sometime the constrast of the image may be quite huge. \
When we have to pre-process the high contrast image, histogram equalization is one of the methods. \
It is useful for images that are almost filled with bright or dark color. \
Histogram equalization uses CDF (Cumulative Distribution Function) to calculate the cumulative probability of different lightness value, \
and spreads out the distribution according to the original one. \
In Python and C++, there is OpenCV to process image with histogram equalization, here I'll implement mine on C++ and compare the efficiency with them. \
![wiki_hist_eq](https://upload.wikimedia.org/wikipedia/commons/c/ca/Histogrammeinebnung.png)

## Prospective Users
The people who want to decrease the contrary of their image, can take advantage of the convenience of Python when reading image \
and the efficiency of C++ when processing the image with histogram equalization.

## System Architecture
![image](https://user-images.githubusercontent.com/92377801/199840940-b57d5bfb-9c62-4bcd-99e6-50b114c04b81.png)

## API Description
- hist_eq(pixel_arr[])
  - provide API for Python to call C++ implementation of histogram equalization
- using OpenMP or intrinsic function to increase the efficiency

## Engineering Infrastructure
1. Automatic build system: CMake
2. Version Control: git
3. Testing Framework: pytest

## Schedule

Week 1 (10/31): Implement Histogram Equalization with C++ (I)

Week 2 (11/7): Implement Histogram Equalization with C++ (II)

Week 3 (11/14): Build automatic compile and unit test

Week 4 (11/21): Refactor code

Week 5 (11/28): Compare the efficiency with OpenCV

Week 6 (12/5): Visualize the result

Week 7 (12/12): Summarize in presentation slide

Week 8 (12/19): Check presentation content

## References
https://en.wikipedia.org/wiki/Histogram_equalization \
https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html
