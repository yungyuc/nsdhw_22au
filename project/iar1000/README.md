# Voronoiagram

## Problem to solve

The Voronoi Diagram $D$ describes a partitioning of a plane $\Omega$ into a set of cells $C$, given a set of target points $T$.
Each cell $c_k \in C$ defines the set of points consisting of one target point $t_k \in T$ and all points in $\Omega$ that are closer to $t_k$ than to any other point in $T$, i.e.

$$
c_k = \{ x \in \Omega | d(x, t_k) \leq d(x,t_q), \forall t_q\in T. t_q \neq t_k\}.

$$

, whereas the metric "closer" is defined by some distance function $d: \Omega \times \Omega \rightarrow \mathbb{R}$.

### Fortune's Algorithm

The algorithm uses a __sweep line__ and a __beach line__ to create and grow new cells of the diagram. The __sweep line__ is a vertical line "moving" in one direction (we assume the direction to be from left to right from now on) through the plane, whereas points passed by the sweep line start growing their cells. The __beach line__ is a piecewise parabolar curve, consisting of a set of Arcs P, tracking cell boarders that are still growing. The beach line describes all points that are equidistant from the sweep line to the nearest target point.

![](https://upload.wikimedia.org/wikipedia/commons/0/0c/Fortunes-algorithm-slowed.gif)

### Events

The implementation itself is not literally sweeping the sweep line through the whole plane and adapting the beach line on it's way, but skipping to locations where important events take place. This is also called an event-based algorithm. The two major events that have impact on the diagram are the following:

1) **Point Event**: The sweep line encounters a new target point, a new arc is added to the beach line tracking the boarder of the new cell
2) **Circle Event**: The length of an arc shrinks to zero, a new vertex in the Voronoi diagram is created from where a new cell boundary starts

#### Point Event

The sweep line encounters target point $t_k$. The horizontal line $h_k$ going through $t_k$ is going to intersect with a parabola $p_k \in P$ to the left of the sweep line. A new parabola is introduced to the beach line at the intersection of $h_k$ and $p_k$. It is infitesimally small in the beginning and starts to grow as the sweep line progresses, tracking the equidistance level curve for the newly incorporated point $t_k$.

As the sweep line progresses only in one direction, we know the order of the point events in advance. Sorting the target points in advance allows us an efficient lookup for the next point event.

#### Circle Event

Circle events are created on the beach line by three neighbouring target points. Each of the three points has an arc associated with it, all part of the beach line, which indicates equidistance from the closest target point and the sweep line. While all of the target points grow their cell, their respective arcs on the beach line grow with them, until one of them is "squashed" to length zero by the other two. This is going to happen at point $v_i$, which has the same distance to all three neighbours. A Voronoi vertex is created at this location and the parabola with empty length is removed, because we know that in the final diagram, a border is starting at that point.

### Pseudo Code

We keep track of the events in a event queue $Q$, handling them one after the other.

```
insert all Point Events to Q, sorted by x-coordinate
while Q is not empty:
    pop event E
    if E is a Point Event:
        insert new arc to the beach line
        check for new Circle Events
    else:
        create a new vertex in the diagram D
        remove the shrunk arc from the beach line
        delete invalidated events
        check and add new Circle Events
```

## System Architecture

The algorithm is made accessible through the Python interpreter and uses C++ as backend.
The main workflow is as follows:

1) (Py ) Read in target points $T$ from `.csv` or `.json` file
2) (Py ) Define bounding box (min/max coordinates + margin)
3) (Py ) Initiate diagram computation
4) (C++) Setup `EventQueue` with `PointEvents`
5) (C++) Compute diagram
6) (C++) Return diagram as list of vertices and edges
7) (Py ) Visualize and save diagram

### Datastructures

The main components of Fortune's algorithm are:

1. `VoronoiDiagram`
2. `Beachline`
3. `EventQueue`

The main functionalities are:

1. insert and delete `PointEvent` and `CircleEvent` to `EventQueue`
2. add and delete arc to `BeachLine`
3. Check for new `CircleEvenet`
4. Add vertices and edges to `VoronoiDiagram`

### Testing

The testing environment will be setup in python to help modular development. This also means that the development of the C++ backend goes hand-in-hand with the developement of the `pybind` connector.

## API Description

Will follow when coding has started

## Engineering Infrastructure

* Build: `make`
* Version control: `git`
* Testing infrastructure: `pytest`
* Documentation: `README.md` and inline

## Schedule

Week 1 (10/31): (Py ) read in points, setup testing environment

Week 2 (11/7):  (C++) develop and implement `VoronoiDiagram`, `BeachLine` and `EventQueue` datastructures (w/o functionality)

Week 3 (11/14): (C++) add functionality to `EventQueue` and `VoronoiDiagram`

Week 4 (11/21): (C++) add functionality to `BeachLine`

Week 5 (11/28): (Py ) finish C++ connector and implement module interface

Week 6 (12/5):  (Py ) testing and debugging

Week 7 (12/12): Documentation

Week 8 (12/19): Presentation and buffer for previous weeks

## References

[Reference Project Proposals](https://yyc.solvcon.net/en/latest/nsd/schedule/22au_nycu/schedule.html#voronoi-diagram)
[Guodong Rong (2006), Jump flooding in gpu with applications to voronoi diagram and distance transform](https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.101.8568&rank=1&q=Jump%20Flooding%20in%20GPU%20with%20Applications%20to%20Voronoi%20Diagram&osm=&ossid=)
[Guodong Rong (2007), Variants of Jump Flooding Algorithm for Computing Discrete Voronoi Diagrams](https://ieeexplore.ieee.org/document/4276119)
