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
They can build their graph environment in Python or C++, and export the graph to A* Search Engine.
Then the engine will output the path for the user.

--
Describe the users of your software and how they will use it.  It is OK to
combine this section with the previous one (`Problem to solve`_).

System Architecture
===================

Here is the project work flow. There are three important component in this project: Environment, State, AStarEngine.
State will record the state information, and environment need to know how to read the state data and the state transition rule should be defined to transfer the state to next state.
The AStarEngine do the A* search with the specified enrionment and the initial state.

.. image:: img/work_flow.png

The A* search algorithm can be illustrate as following:



Analyze how your system takes input, produces results, provide interface, and
performs any other operations.  Describe the system's work flow.  You may
consider to use a flow chart but it is not required.  Specify the constraints
assumed in your system.  Describe the modularization of the system.

API Description
===============

User can directly use the CLI to enjoy the basic example game (or environment) to experience the A* search engine.

Or if they want to try the other game, they can implement their game environment derived from the environment class interface and override the method:

:math:`state state_transition(state, action)`

  state_transition defines how to transfer the current state into next state by appling an action. The first parameter is the current state, the second parameter is the desired action.

:math:`action[] valid_actions(state)`

  valid_actions return all the valid actions on the input state.

:math:`float astar_heuristic(state)`

  astar_heuristic is the needed user defined function used for A* search. This function is very important and will affect how quickly will A* search find a path.
  In different game, there would be a different heuristic functions. User can define their own heuristic function for their own game.

After defining the game environment, he/she should set the environment instance and the initial state for A* search engine as following:

:math:`AStarEngine.setup(environment, state)`

  The setup method will help engine to get the environment instance and the initial state, those information is used for the next step.

Then execute the A* search:

:math:`AStarEngine.run()`

  This method will start to execte A* search and find the path for user.

---
Show how your system can be programmed.  You are supposed to implement the
system using both C++ and Python.  Describe how a user writes a script in the
system.

Engineering Infrastructure
==========================

To build the A* Search Engine in c++, I would like to use CMake to automatically build my project.
Version Control: Git. Because this project will be written only by myself, I will use only one branch on development, and the master branch to be released branch.
Test Framework: GTest for C++, pyUnit for Python
The Documentation tool would be a rst file or markdown.
I will wrap an environment into a docker container image, as the result, user can easily pull the image and enjoy the AStarEngine immediately.

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

* Planning phase (6 weeks from 9/19 to 10/31): prepare proposal, study A* search and create repository
* Week 1 (10/31): build the project docker image and project prototype
* Week 2 (11/7): define the environment base interface
* Week 3 (11/14): implement the example game
* Week 4 (11/21): unit test for example game
* Week 5 (11/28): implement the A* search engine
* Week 6 (12/5): unit test for A* search engine
* Week 7 (12/12): system test for entire project
* Week 8 (12/19): prepare presentation

References
==========

List the external references for the information provided in the proposal.