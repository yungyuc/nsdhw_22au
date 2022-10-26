# 3D-Retro-Bouncing-DVD

## Basic Information
Bouncing DVD is a retro screensaver that would appear in people's childhood. When taking tour bus and watching movie via DVD, we could see the screensaver due to device idling for a while. My object is to reproduce a DVD screensaver but in 3D space. Moreover, I'll try to make the interface to allow users create new DVD plate at any position.

![](https://i.imgur.com/EYZdwQy.gif)

## Problem To Solve

First, we might simply this project into a collision simulator. However, balls should be transformed into a plate (words of DVD). The original physical functions need to consider non-uniform surface.

For user interface, I'll use pygame to utilize a window show the 3D space. Users could use mouse to drag and click to generate a new DVD plate. The library of pygame is intent to render 2D object, so PyOpenGL would specify 3D object in this project. Therefore, I must figure out what value or how many variable would be sent to C++ defined function. If there are extremendous things to calculate, the system will not handle.

## System Architecture
![](https://i.imgur.com/jZmPbL5.png)

## API Description
After finishing each function, I would update this block.

## Engineering Infrastructure

- Automatic build system: `cmake`
- Version control: `git`
- Testing framework: `pytest`
- Documentation: GitHub `README.md`


## Schedule

- Week 1 (10/31): Implement 3D ball collision simulator with C++ 

- Week 2 (11/7): Transfer object into non-ball object in simulator

- Week 3 (11/14): Wrap function via pybind and decide what value of DVD state would be as parameter and return value

- Week 4 (11/21): Pygame structure (create window, simple object...)

- Week 5 (11/28): Render 3D space and object via PyOpenGL

- Week 6 (12/5): Combine object defined with PyOpenGL and utilize C++ defined function

- Week 7 (12/12): User inteface via mouse dragging (if there is no bug from previous works to deal with)

- Week 8 (12/19): Prepare slides and presentation


## References

- [ball collision simulator by PyOpenGL](https://github.com/MatheusBurda/BallCollision)
- [2D DVD bouncing](https://github.com/George-lewis/DVDBounce)
- [ball collision by c++](https://cplusplus.com/forum/beginner/270674/)
- [PyOpenGL](https://pyopengl.sourceforge.net)