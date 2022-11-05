# Self-Driving Car Localization
#. `kexuan5458 <https://github.com/kexuan5458>`__ for
   `Self-Driving Car Localization <kexuan5458/README.md>`__: https://github.com/kexuan5458/Self-Driving_Car_Localization

## Basic Information

Kalman Filter and Iterative Closest Point(ICP) algorithm are good tools in the self-driving car localization task.

## Problem to Solve

One of the biggest challenges of self-driving car's tracking and control systems is providing an accurate and precise estimation of the hidden states, location and velocity, in the presence of uncertainty. There are numerous sensors on the self-driving cars that estimate hidden states based on a series of measurements . The Kalman Filter can fuse information from different sensors, and produce estimates of hidden variables based on inaccurate and uncertain measurements.  

In this project, I will develop a localization module for estimating the poses of a self-driving car with a given map. The algorithm will be based on the ICP.

## Goal

1. Implment a C++ localization algorithm.
2. Try to get better localization result.
3. Use pybind to make it easier to show the result of different map by Python.

## Prospective Users

People who wants to compare different localization result more easily.

## System Architecture

The project will be made accessible through the Python interpreter and uses C++ as backend.
The main workflow is as follows:

1. Use `C++` to implement localization algorithm.  
2. Design the interface for users reading the car pose in map frame by `Python`  
3. Use pybind11 to make it callable by python.

## Test  
The testing environment will be setup in Python. So, the development of the C++ backend goes with the developement of the pybind connector.

## API Description

Return the location and velocity data of each frame. And show the accuracy or plot the comparison results.
It would be updated after coding has started.

## Engineering Infrastructure

1. Build system: `make`、`ROS`
2. Version control:  `git`
3. Testing framework: `pytest`
4. Documentation: `README.md`
5. Environment: `Ubuntu 20.04 (Docker)`

## Schedule

* Planning phase (6 weeks from 9/19 to 10/31):
* Week 1 (10/31): Setup ROS & C++ environment
* Week 2 (11/07): Implement the Kalman filter on a real data (C++)
* Week 3 (11/14): Add ICP in my algorithm (C++)
* Week 4 (11/21): Visualize and save data (Python)
* Week 5 (11/28): Improve result and make test
* Week 6 (12/05): Debug and make sure the system work
* Week 7 (12/12): Buffer time for testing and debugging
* Week 8 (12/19): Prepare presentation

## References
[My Project](https://github.com/kexuan5458/Self-Driving_Car_Localization)  
[SDC-EKF](https://github.com/kharikri/Self-Driving-Car-Extended-Kalman-Filters)  
[PCL (ICP)](https://github.com/PointCloudLibrary/pcl)    
[tf](http://wiki.ros.org/tf)  
[Direction Cosine Matrix](http://www.starlino.com/dcm_tutorial.html)   