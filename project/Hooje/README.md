Simple Parallel Decision Tree in C++
====

# Basic Information
Traditional Machine Learning models are still effective ways for classification.
And Decision Tree is one of them.

Decision Tree is a series a node that start with one root node and extends to many nodes that can represents the classfication rules for the data.
For a dataset, Decision Tree will calculate the impurity (GINI index, entropy...) of every possible split, and choose the best one to be the rule of root node and create two leaf nodes.
For each node, use the same method to decide the rule and create its child nodes.


# Problem to Solve
Decision Tree is an effective model in many classification senerio. However, it takes so much time to run because it cannot be speedup by CUDA like Neural Network, especially when there are a lot of features in that Dataset.


Thus, I want to implement a simple version of Decision Tree in C++ (can be used from Python) from scratch to reduce the execution time. 
Besides, I will compare the results between with and without SIMD.
After that, I will try to finish multi-thread version and compare it with them.

# Prospective Users
Those people who use traditional ML models can benefit from my Decision Tree. 
They can spent less time waiting for the model to finish.

# System Architecture
There will be a Dataset class, which will include features and its value.
And a Node class, which represents each node in the Tree and connects with its parent node and child node. 
And a Decision Tree Class, which will be the whole structure of Decision Tree and provide functions of Deicsion Tree.
Users can directly use it in C++ environment.
They can also use the Class from Python to call C++.

# API Description
When users prepare their dataset (training set and testing set), they can use 'fit' function to train their model and use 'predict' to predict the results for new data (testing set).

# Engineering Infrastructure
I will use Makefile to design an automatic build system to build my program.
And I will use git to do the version control.
After finishing this project, I will write a simple documentation to describe how to run my program.



# Schedule
Week 1 (10/31): Function for input dimension and structure.

Week 2 (11/7):  Finish original 'fit' function and function of calculating impurity.

Week 3 (11/14): Finish original 'predict' function.

Week 4 (11/21): Change original function into parallel function.

Week 5 (11/28): Use Datset to run and compared with the results from Deicison Tree in Python library.

Week 6 (12/5): Write Python code to use it. 

Week 7 (12/12): Refactoring week.

Week 8 (12/19): Prepare presentation.


