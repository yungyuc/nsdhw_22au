# KD-Tree

## Basic Information
K-D tree (k-dimensional tree) is a space-partitioning data structure for organizing points in a k-dimensional space. k-d trees are a useful data structure for several applications, such as searches involving a multidimensional search key (e.g. range searches and nearest neighbor searches).

![](https://i.imgur.com/0VUKaEH.gif=500x)

Find the minimum distance in kd-tree.

## Problem to solve
In computational geometry, it is often necessary to find a point or line segment in a specific block among a bunch of points or line segments on the coordinate plane. For example, I want to find all the points on the two-dimensional coordinate plane that fall in the rectangular area of (0, 0) ~ (10, 10).

There are several commonly used data structures: R-Tree, K-D Tree, Quad Tree, BVH.

K-D Tree is a binary tree in which every node is a k-dimensional point. It continuously divides the dimensions in the space, and uses the characteristics of search tree pruning to shorten the time complexity.

## Prospective Users
For those who want to do range searches and nearest neighbor searches in multidimensional data.

## Engineer Infrastructure
* Environment: `Ubuntu20.04`
* Automatic build system: `cmake`
* Version control: `git`
* Testing framework: `pytest`
* Documentation: `GitHub README.md`

## System Archtecture

## API
### Python
- `Load_data('data.txt')`
    - load data points from data.txt
### C++
- `Build_Tree(int k-dimension, node_set)`
    - build a k-d tree based on the the dimension size and the data point
    - every node in node_set is a k-dimensional point
- `Find_min_distance(target_node)`
    - find the minimum distance between the target node and one of the node in k-d tree
- `Search(min/max, which_dimension)`
    - search the the minimum/maximun in dimension in k-d tree
- `Range_search([x1, x2], [y1, y2])`
    - find all the points that are in the given range
- `Insert_node(node)`
    - insert a node in k-d tree
- `Delete_Node(node)`
    - delete a node in k-d tree

## Schedule
* Week 1 (10/31): Survey k-d tree & Implement the k-d tree with C++
* Week 2 (11/7): Implement `Build_Tree`, `Find_min_distance`, `Search` in k-d tree with C++
* Week 3 (11/14): Implement `Range_search`, `insert`, `delete` in k-d tree with C++
* Week 4 (11/21): Make python wrappers for C++ with pybind
* Week 5 (11/28): Test all features with pytest
* Week 6 (12/5): Finish up, debug, and write the documentation
* Week 7 (12/12): Make slides and prepare for the presentation
* Week 8 (12/19): Make slides and prepare for the presentation

## References
- https://commons.wikimedia.org/wiki/File:Kdtree_animation.gif
- https://en.wikipedia.org/wiki/K-d_tree
- https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/kdtrees.pdf
