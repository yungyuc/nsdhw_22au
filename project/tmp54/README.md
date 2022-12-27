# R-Tree Search Engine

## Problem to Solve

R-Tree is a tree data structure used to represent a set of object in an n-dimensional space. Each node of the tree is the Minimum Bounding Rectangle (MBR) of the child nodes. The leaf node is an object in the n-dimensional space.

## System Architecture

- Builder pattern
- Fluent API

```cpp
class RTree {};

class Node {};
```

## API Description

### Initialize an Instance

```python
# create a r-tree in 2D
r_tree = RTree(2)
```

### Insert Object

```python
# insert a rectangle which its left-bottom and top-right corner is at (0, 0) and (10, 10), respectively
r_tree.insert_object(0, 0, 10, 10)
```

### Delete Object

```python
# delete a rectangle which its left-bottom and top-right corner is at (0, 0) and (10, 10), respectively
r_tree.delete_object(0, 0, 10, 10)
```

### Query

```python
# return the number of the objects that are intersect with such rectangle
r_tree.intersection_count(1, 1, 2, 2)
```

## Engineering Infrastructure

- Build system: Makefile (or CMake)
- Version control: git
- Testing framework: pytest
- Linter: ClangFormat, Pylint
- Documentation: docstring (hopefully Sphinx)

## Schedule

- ~3 weeks: finish R-Tree, including
  - C++
  - Python binding
  - Build system
  - Testing
- Until presentation: work hark
