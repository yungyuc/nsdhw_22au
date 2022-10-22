#  LSD (Line Segment Detector)
## Introdution
In computer vision region, detection of the line on the images in real time is an useful function, because it has a lot of downstream task, such as vanish point method base camera calibration and road marking segmentation.

![](https://i.imgur.com/WD8w34b.png)

reference from [2](https://www.ipol.im/pub/art/2012/gjmr-lsd/article.pdf)
## Problem to solve
1. The time of line detection should be as fast as possible.
2. The detection result should be visualized.


## Prospective Users
The people who want to get line information on the image and need a fast algorithm. 


## System Architecture
OS imformation: ubuntu 22.04

flow chart below ( TODO )

## API Description
1. Load image function
2. Output image function 
3. Line detection algorithm function
```
# This is the pseudo code
# All function will wrap in python function 

def main():
    ori_img = load_image(IMG_PATH)
    lsd = LSD(Hyperparameter...)
    
    # line_list could do more application in downstream task.
    line_detection_result,line_list = lsd.detect(ori_img)
    save_img(line_detection_result, SAVE_PATH)
```
## Engineering Infrastructure
Automatic build system: make

Version control: git


Testing framework: [pytest](https://pytest.org)

Documentation: [doxygen](https://doxygen.nl/) For automatic document generation.

## Schedule
Week 1 (10/31): Study algorithm and search third party library.

Week 2 (11/7): Complete make file and image I/O control.

Week 3 (11/14): Complete testing part and image preprocess function.

Week 4 (11/21): Implement the line detection algorithm.

Week 5 (11/28): Code review for speed up(Maybe use BLAS).

Week 6 (12/5): Optimize the performance.

Week 7 (12/12): Complete documentation (Maybe trun it into the web page).

Week 8 (12/19): Prepare the presentation.

## References
1. https://www.ipol.im/pub/art/2012/gjmr-lsd/article.pdf
2. https://ieeexplore.ieee.org/document/4731268
