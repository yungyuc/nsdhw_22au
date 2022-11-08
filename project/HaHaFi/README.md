#  LSD (Line Segment Detector) 
## Introdution
In computer vision region, detection of the line on the images in real time is an useful function, because it has a lot of downstream task, such as vanish point method base camera calibration and road marking segmentation.




**Alogorithm**
![](https://i.imgur.com/EXi6z32.png)

reference from [2](https://www.ipol.im/pub/art/2012/gjmr-lsd/article.pdf)
## Problem to solve
1. The implementation of this project should be the linear time algorithm as the same as in [paper](https://ieeexplore.ieee.org/document/4731268).
3. The detection result should be visualized and compare to others line detection algorithms.


## Prospective Users
The people who want to get line information on the image and need a fast algorithm.


## System Architecture
* OS imformation: ubuntu 22.04
* Architecture: amd64





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
week 0(10/24): Study algorithm and search third party library.

Week 1 (10/31): Study algorithm and search third party library.

Week 2 (11/7): Complete make file and implement the line detection algorithm.

Week 3 (11/14): Implement the line detection algorithm.

Week 4 (11/21): Complete testing part and use third party library to implement others line detection algorithm to compare.

Week 5 (11/28): If the schedule is not delay, do the code review for speed up(Maybe use mkl library) .

Week 6 (12/5): Optimize the performance and Complete documentation (Maybe trun it into the web page).

Week 7 (12/12): Prepare the presentation.

Week 8 (12/19): Prepare the presentation and make the speech draft.

## References
1. https://www.ipol.im/pub/art/2012/gjmr-lsd/article.pdf
2. https://ieeexplore.ieee.org/document/4731268

## Repository 
https://github.com/HaHaFi/Line-Segment-Detection