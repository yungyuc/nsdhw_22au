========================
A Star Search Engine
========================
.. note::

  You are strongly encouraged to use this plain-text `reStructuredText
  <https://docutils.sourceforge.io/rst.html>`__ format, although not forced.
  :download:`Download this file. <project_template.rst>`

Basic Information
=================

My Project Link: `<https://github.com/DerrickPikachu/AStarEngine>`

Problem to Solve
================

Path Finding problem is really famous in Computer Game AI. Maze, Sliding Puzzle, or Magic Cube are Path Finding games. Also in mordern video games, to find the
path of the NPC is a Path Finding problem.
To takle this problem, there are some algorithm we may come up with, like Dijkstra and Breadth-First-Search.
However, Breadth-First-Search waste too much time on searching in breadth.
A* Search, which combine the feature of Dijkstra and Best-First-Search, can efficiently find the shortest path. 
This project provide a C++ API for Python.
User who want to utilize the A* Search can build the game (or environment) in Python script and call the A* Search Engine to help him/her finding path.
--
Describe the problem or the set of problems you want to solve.  Include
necessary background information without making it lengthy.

Some points may help you organize the problem description:

1. The field or industry of the problem.
2. The physics and/or the mathematics behind the problem.
3. The algorithm or numerical method that should be applied for solving the
   problem.

Prospective Users
=================

The programmer who want to find a path on a graph or want to solve a Path Finding problem.
They can build their graph environment in Python, and export the graph to A* Search Engine.
Then the engine will output the path for the user.
--
Describe the users of your software and how they will use it.  It is OK to
combine this section with the previous one (`Problem to solve`_).

System Architecture
===================

Analyze how your system takes input, produces results, provide interface, and
performs any other operations.  Describe the system's work flow.  You may
consider to use a flow chart but it is not required.  Specify the constraints
assumed in your system.  Describe the modularization of the system.

API Description
===============

Show how your system can be programmed.  You are supposed to implement the
system using both C++ and Python.  Describe how a user writes a script in the
system.

Engineering Infrastructure
==========================

To build the A* Search Engine in c++, I would like to use CMake to automatically build my project.
Version Control: Git. Because this project will be written only by myself, I will use only one branch on development, and the master branch to be released branch.
Test Framework: GTest for C++, pyUnit for Python
The Documentation tool would be a rst file or markdown.
--
Describe how you plan to put together the engineering system:

1. Automatic build system and how to build your program
2. Version control (show how you will use it)
3. Testing framework
4. Documentation

Some of the above information should be included in the documentation in your
software.

You may use continuous integration, but it is not required.  If you use it,
describe how it works in your code development.

Schedule
========

* Planning phase (6 weeks from 9/19 to 10/31): prepare proposal and create repository
* Week 1 (10/31): study A* search
* Week 2 (11/7): build the project docker image and project prototype
* Week 3 (11/14): implement the example game
* Week 4 (11/21): unit test for example game
* Week 5 (11/28): implement the A* search engine
* Week 6 (12/5): unit test for A* search engine
* Week 7 (12/12): system test for entire project
* Week 8 (12/19): prepare presentation

References
==========

List the external references for the information provided in the proposal.