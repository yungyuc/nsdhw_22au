# TSP with gene algorithm

## Basic information
TSP (Travelling salesman problem) is a problem proved to be NP-hard, so we cannot use brute force to find the optimal solution.

## Problem to solve
TSP alogrithm intended to find the best route to which visits all cities once and returns to the origin city.

## Prospective Users
For those who would like to save time or money while traveling cities.

## System Architecture
1. Generate a random route
2. Randomly generate specific number of routes with currently best route
3. Calculate the Cost of each route
4. Repeate step 2 & 3 specific times

# API Description

## Engineering Infrastruction
1. Automatic build system: `GNU make`
2. Version control: `git`
3. Testing framework: `pytest`
4. Documentation: `README.md`

## schedule
* Planning phase (6 weeks from 9/19 to 10/31): Study domain knowledge
* Week 1 (10/31): Setup linux & C++ environment
* Week 2 (11/7): Generate graph
* Week 3 (11/14): Build TSP algorithm using gene algorithm
* Week 4 (11/21): Speedup gene algorithm with multi-thread
* Week 5 (11/28): Complete testing
* Week 6 (12/5): Debug and make sure the system work
* Week 7 (12/12~): Prepare presentation

## Reference
- [TSP](https://en.wikipedia.org/wiki/Travelling_salesman_problem)
