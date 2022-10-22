# Celluar Automata 3D

## Basic Information

A software to simulate and visualize 3D cellular automata
* github repository: [https://github.com/ian-chiu/celluar-automata-3d](https://github.com/ian-chiu/celluar-automata-3d)

## Problem To Solve

Like the 2D Cellular Automata (Conway's Game of Life), the goal is to simulate and visualize cellular automata but extend from 2D to 3D.

| ![Conway's Game of Life](https://upload.wikimedia.org/wikipedia/commons/e/e5/Gospers_glider_gun.gif) | ![](https://i.imgur.com/nQ7m3OU.png) |
|:--------:|:--------:|
| **2D Cellular Automata - Conway's Game of Life** | **3D Cellular Automata - from Softology's Blog** |

Cellular Automata Definition from Tech Target:
> A cellular automation is a collection of cells arranged in a grid of specified shape, such that each cell changes state as a function of time, according to a defined set of rules driven by the states of neighboring cells.

### Cell Rules

Cell rules can be represented by: 
**Survival / Spawn / State / Neighbor**

#### Survival

A list of acceptable numbers of neighbors that are alive (in state 1) to keep a cell survived (stay in state 1). If the number does not match any in the list, the cell start dying (its state start increasing every tick). If a cell's state is greater than its maximum state, it died (back to state 0).

#### Spawn

A list of acceptable numbers of neighbors that are alive (in state 1) to spawn a new cell. If a cell is empty (in state 0) and the number matches any in the list, the cell get spawned (go from state 0 to 1).

#### State

The maximum number of state a cell can have. If a cell's state is greater this number, then it died (back to state 0).

#### Neighbor

There are two kinds of neighbor, Von Neumann(VN) and Moore(M). Von Neumann is like a rubik cube, the cell at the center has 26 neighbors. Moore has only 4 neighbors, top, down, front and back.

### Optimization

#### Multithreading and SIMD... or any parallel computing techinques

Use parallel computing techinques, like multithreading and SIMD operations, to increase computation speed of calculating all cell states in the next frame under a rule. 

If a grid is 100x100x100, we have to calculate next states of 1000000 cells according to their neighbors every frame, which needs a lot of computation.

#### Greedy Meshing Algorithm

The idea of the algorithm is combining adjacent blocks to reduce the part count while visually keeping everything the same, which is useful to reduce draw calls. 

For a 100x100x100 grid, we have to draw maximum 1000000 cubes if all cells are not empty. If the number of draw calls is too large, the communication between cpu and gpu will be overwhelming, which can greatly reduce the performance of rendering.

![](https://i.imgur.com/SCIFdNj.png)

## Prospective Users

### Biology

Several biological processes and systems are simulated using Celluar automations, including the patterns of some seashells, moving wave patterns on the skin of cephalopods and the behaviors of brain neurons.

### Physics

Celluar automata are used to simulate and study physical phenomena, such as gas and fluid dynamics.

### Cryptography

Celluar automata are proposed for use in public key cryptography. They can be utilized to construct pseudorandom number generators, and to design error correction codes.

### Generative Art and Music

Cellular automata have been used in generative music and evolutionary music composition and procedural terrain generation in video games.

*Sources*:
* Tech Target: https://www.techtarget.com/searchenterprisedesktop/definition/cellular-automaton
* Wikipedia:
https://en.wikipedia.org/wiki/Cellular_automaton#Applications

## System Architecture

### Features

The software should have following features:

1. Simulation operations: randomize, run, pause, clear
2. Simulation settings: randomize density and range, grid size
3. Some cell rules can be used by default
4. Users can add their own cells rules
5. Basic camera control: Orbiting and Zooming

### Input / Output

* Input: GUI, keyboard, mouse
* Ouput: application window

### Third Party Library

- raylib-py or pygame: A simple video games programming library for dealing with windows, keyboard/mouse input, graphics and GUI.

## API Description

### Python

* Rule: 
    * apply: apply the cell rule
    * count_neighbors: get number of alive neighbors according to the rule
* CameraController:
    * update: update camera view and projection matrix according to the control input
* Application:
    * run: control the render loop and the all update for the next frame
    * randomize: randomize all cells in a grid
    * clear: empty all cells in a grid

### C++

* calculateNextState: calculate all cells' states of the next frame. Should use multithreading and SIMD or any parallel computing techniques to increase computation speed.
* getGreedyMeshes: get the merged meshes after executing the greedy meshing algorithm

## Engineering Infrastructure

|Category|Tools|
|--|--|
|Build System|CMake|
|Testing Framework|pytest|
|Version Control|Git|

## Schedule

### Planning phase (6 weeks from 9/19 to 10/31): 

- Learn some parallel computing techniques, like multi-threading and SIMD operations. 
- Get familiar with the greedy meshing algorithm and how to use the third party library.
- Create the application window and get the render loop running. 
- Render a cube on the window.

### Week 1 (10/31): 

- Make sure the keyboard and mouse input can work.
- Finish camera controller.

### Week 2 (11/7):

- Setup the grid.
- Implement cell rules in python.
- Implement calculateNextCells in C++ without optimization.
- Make sure cells in the grid behave correctly in every iteration according to the applying rule.
- Write a python test for a simple cell rule.


### Week 3 (11/14):

- Create the GUI.
- Implement all the simulation operations and settings that needs GUI.

### Week 4 (11/21):

- Optimize computing speed of calculateNextCells by using the parallel computing techniques, like SIMD or multi-threading. 

### Week 5 (11/28):

- Implement the getGreedyMeshes to reduce draw calls.
- Test getGreedyMeshes works correctly.

### Week 6 (12/5):

- Finish up, debug, and write the documentation

### Week 7 (12/12):

- Finish up, debug, and write the documentation
- Make slides and prepare for the presentation

### Week 8 (12/19):

- Make slides and prepare for the presentation

## References

* Tech Target: https://www.techtarget.com/searchenterprisedesktop/definition/cellular-automaton
* Wikipedia:
https://en.wikipedia.org/wiki/Cellular_automaton#Applications
* How Greedy Meshing Works:
https://devforum.roblox.com/t/consume-everything-how-greedy-meshing-works/452717
* Softology Blog - 3D Cellular Automata:
https://softologyblog.wordpress.com/2019/12/28/3d-cellular-automata-3/