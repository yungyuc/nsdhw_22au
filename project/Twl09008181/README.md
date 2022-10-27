# KMeans implementation

## Basic Information

KMeans is a famous algorithm for clustering, it is easy to implmenet and use.        
It is widely used in data analysis and machine learning sphere.   
A lot of algorithm is based on this simple and classical algorithm, like Spectral Clustering or data Preprocessing for reducing the complexity for latter training.   
Generally, you can use KMeans when you want to group some data with similar attributes.   


## Problem to solve 

Kmeans pseudo code:

```
K-means(D,k,epsilon){
randomly initilize k Centroids vector :u1,u2,..uk

repeat until convergence:
  set Centroids set empty for C1,C2,..Ck.
  
  //assign to Centroid 
  for each data in D:
    caculate the distance between data and Centroids(u1,u2,...uk)
    assign data to the closest centroids Cj.
  
  //update Centroid vector 
  foreach Cj in Centroids:
    uj = 0
    for each data in Cj:
      uj += data
    uj /= |Cj|

output the Centroids.
}

``` 

KMeans is simple but very computationally intensive,    
It can be splitted into two major steps:    
1. Assign Step : to cacaulate distance between data and all centroids.    
2. Centroid Updating Step: to update the coordinate of each centroid according to assignment step.      

I think it's worthy to speed up kmeans by C++, and make it a general usage for python programmer.     
The interface of kmeans is simple, we can just use 2D array to represent the dataset.     
It's easy to get dataset from sklearn.    


## Goal

1. Implment a C++ version KMeans.
2. Try to speed up with parallel programming skill.
3. Use pybind to make it callable by python.
4. Comparing the efficiency with the sklearn kmeans model.


## Prospective Users
C++ or python programmer who want use k-means as a step to build their algorithm.

## System Architecture


API

```
//group to K-centroid, the return value in range [0,K-1] to indicate which group it belong to.
vector<int> kmeans(vector<vector<float>>&dataset, int K, double epsilone);
```

1. C++/Python
2. Pybind
3. handcraft thread pool or OpenMP.

## Engineering Infrastructure     

Build System : make     
Version Control : git     
Test : pytest or goolge test      


## Schedule   

week0(11/01): sequential kmeans algorithm study.          
week1(11/07): Implement sequential kmeans by C++ and try to use pybind to call by python.   
week2(11/14): parallel kmeans algorithm study.      
week3(11/21): parallel programming framework study.     
week4(11/28): Implement parallel kmeans by C++.       
week5(12/05): Testing and compare with sklearn kmeans.        
week6(12/12): Buffer time for checking speedup or add another features.     
week7(12/19): Prepare slides.     
week8(12/26): Project Present     
  
## References
Parallel K-Means Clustering Based on MapReduce:https://link.springer.com/chapter/10.1007/978-3-642-10665-1_71

