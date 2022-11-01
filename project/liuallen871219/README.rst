# Iterative Closest Point

## Basic Information
My ptojrct link<[https://github.com/liuallen871219/ICP](https://github.com/liuallen871219/ICP)>
## Problem to Solve
Iterative closest point (ICP)is an algorithm employed to minimize the difference between two clouds of points. ICP is often used to reconstruct 2D or 3D surfaces from different scans, to localize robots and achieve optimal path planning (especially when wheel odometry is unreliable due to slippery terrain), to co-register bone models, etc.

## Prospective Users
The users who want to find transformation between two point sets. If the users have serise of rgb and depth image, they can use the ICP to do the 3D reconstruction of scene.
## System Architecture
![](https://i.imgur.com/uJfzY04.png)

## API Description
### Python
```icp()``` :  return transformation between two pointsets
### C++
```dist()``` : return distance between two point
```nearest_neighbot()``` retrun nearest_neighbot in two pointsets
```best_fit_transform()``` return transform between two pointsets by SVD
```icp()``` perform whole icp procudure

## Engineering Infrastructure
* Building tool: make
* version control: git
* testing: pytest
## Schedule
Week 1 (10/31): Study ICP algorithm

Week 2 (11/7) : Implement the `dist(),nearest_neighbot(),best_fit_transform()` in C++

Week 3 (11/14): Finish C++, and Make python wrappers for C++ with pybind

Week 4 (11/21): Test all features with pytest

Week 5 (11/28): Compare time with other libraries

Week 6 (12/5):  Finish up, debug, and write the documentation

Week 7 (12/12): Buffer time for further testing and debugging

Week 8 (12/19): Make slides and prepare for the presentation
## Reference
[https://en.wikipedia.org/wiki/Iterative_closest_point](https://en.wikipedia.org/wiki/Iterative_closest_point)