# Clip your arbitrary polygons

## Basic Information
Clipping polygons means using a polygon to clip a subject one. Only intersection parts of both would be retain. This function would be used to image processing or clipping a image into certain shape.

> ![](https://i.imgur.com/jugvja9.png)
>
> **Clipping of polygons**

## Problem to solve
Some algorithms to solve clipping polygons might be fast but have some limit of input (ex. polygons have to be convex). Greiner–Hormann clipping algorithm can be fast and easy than other algorithm. Furthermore, it doesn't limit the shape of polygons. Therefore, I decide to reproduce the polygon clipping function with this algorithm (Greiner–Hormann clipping algorithm) in C++. Both clip and subject polygons could be concave or convex.


## Engineer Infrastructure
* Environment: `Ubuntu22.04`
* Automatic build system: `cmake`
* Version control: `git`
* Testing framework: `pytest`
* Documentation: `GitHub README.md`

## System Archtecture

> ![](https://i.imgur.com/2jn4i96.png)
>
> **block diagram of system**

- **User input process** : I would choose `OpenGL` to display polygons and ask users to input clip and subject polygons. The interface is the window where `OpenGL` shows the result. Users decides the count of vertices of two polygons then point vertices one by one on window via mouse. So I should save the position(x, y) of vertices in each polygon and display the input polygons immediately.

- **Main algorithm of clipping polygons** : After fetching two sets of vertices' position, I would implement Greiner–Hormann clipping algorithm to generate which parts of subject polygon should be remain and record the vertices of new polygon(intersect parts).

- **Display result** : `OpenGL` use the result vertices to display intersect parts on window.

## API

### C++
- input_polygons (int clip_polygon_vertices_num, subject_polygon_vertices_num)
  - Start from OpenGL, create a window and ask user to point out the vertices in window according to count of two polygons 
  - first and last vertices should be close enough, or there would exit with error
  - calibrate first and last vertices, if they are not perfectly overlap
- fetch_vertices (void)
  - create two vectors to store (x, y) of vertices of two polygons
- vector<pair(int, int)> clipping_polygons (vector<pair(int, int)> clipping_poly, vector<pair(int, int)> subject_poly)
  - main block of algorithm, return the overlapping part
- show_result (vector<pair(int, int)> result)
  - clear input (two polygons) in window, then show the result polygons

## Schedule
* Week 1 (11/7): Comprehend Greiner–Hormann clipping algorithm by the paper.
* Week 2 (11/14): Realize this method in C++ with different instance of polygons.
* Week 3 (11/21): Wrap the function into a module, utilize makefile to automatic build and make some easy tests with `pytest`.
* Week 4 (11/28): Build `OpenGL` environment and generate different types of polygon with serial input vertices(x, y).
* Week 5 (12/5): Build user iterface to generate polygons by clicking the window. 
* Week 6 (12/12): Combine both features above and debug.
* Week 7 (12/19): Prepare slides and presentation.

## References
- https://www.inf.usi.ch/hormann/papers/Greiner.1998.ECO.pdf
