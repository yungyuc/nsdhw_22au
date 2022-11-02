# t-SNE implementation



## Basic Information

t-SNE (t-distributed stochastic neighbor embedding) is an dimension reduction algorithm.  It can be used in visualizing high dimension  data and feature reduction.  **By the way, I will compare my implementation with sklearn.**

- github repository: https://github.com/LanceWang12/t-SNE-implementation



## Problem to solve

In the Big Data analysis, data is not only becoming bigger and bigger; but also becoming more and more complicated.  It causes a spectacular increase of the dimensionality of the data.  For example, the dimensionality of a set of images is the number of pixels in any image, which ranges from thousands to millions.

Computer has no problem processing that many dimensions.  However, we are limited to three dimensions.  Computer still needs us, so we should find a way to effectively visualize high-dimensional data before handing it over to the computer.

There are many methods to solve this problem.  For example, PCA, LDA, Kernel method, Random projection and so on.  t-SNE uses Gaussian distribution to approximate high dimentionality and uses t distribution to approximate low dimentionality.  It uses KL-divergency to compute similarity and uses gradient discent to get the optima.

![](https://i.imgur.com/cBQO9oV.gif)

(using MoviePy to produce animation)



## System Architecture

![](https://i.imgur.com/XOAw7qx.png)



## API Description

```c++
// Initialize hyperparameter for t-SNE object
int x[10][3] = [
  [1, 2, 3],
  [4, 5, 6],
  ...
]
tSNE(
  int n_components=2, 
  double perplexity=30.0,
  double dearly_exaggeration=12.0,
  int learning_rate=200,
  int n_iter=1000,
  int n_iter_without_progress=300,
  double min_grad_norm=1e-07,
  string metric="euclidean",
  string init="random",
  bool verbose=0,
  int random_state=NULL,
  string method="barnes_hut",
  double angle=0.5,
  int n_jobs=NULL,
)
  
X_embed = tSNE().fit_transform(X)
// X_embed = [
//  [0.1, 0.2],
//  [0.3, 0.4],
//  ...
// ]
```



## Engineering Infrastructure

- Automatic build system: `cmake`
- Version control: `git`
- Testing framework: `pytest`
- Documentation: GitHub `README.md`

## Schedule

Planning phase : 

Week 1 (10/31): Implement the t-SNE algorithm with C++

Week 2 (11/7): Make python wrappers for C++ with `pybind`

Week 3 (11/14): Finish C++, and start visualizing some data in python

Week 4 (11/21): Implement features of visualization

Week 5 (11/28): Test all features with `pytest`

Week 6 (12/5): Finish up, debug, and write the documentation

Week 7 (12/12): Buffer time for further testing and debugging

Week 8 (12/19): Make slides and prepare for the presentation



## References

https://scikit-learn.org/stable/modules/generated/sklearn.manifold.TSNE.html

https://github.com/pybind/pybind11