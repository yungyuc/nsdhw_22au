========================
Contiguous Array
========================

Basic Information
=================

https://github.com/soulrrrrr/ColumnarArray

Problem to Solve
================

Contiguous Array is basic for most of scientific calculations. Espicially in machine
learning area. I want to implement my version of it, and I hope it helps people
dealing with problems that need to use contigious array and matrix.

Prospective Users
=================

People wants lighter library for basic array and matrix operations.

System Architecture
===================

A class template :cpp:class:`!Array` is the contigious array. There are several methods
to initialize the array and use them. Each method will be exposed to Python by pybind11
wrapper.

At the moment I am planning, :cpp:class:`!Array` can only deal with 
structs have numeric types only.

API Description
===============

:cpp:class:`!Array` will have a constructor, user can pass python list
into the constructor, and it will be the array of the list.

:cpp:class:`!Array` will provide a lot of methods to do things with array.
Such as *zeros*, *ones*, *arange*, and basic numeric operatons ("+", "-",
"*", "@"(dot)).


Engineering Infrastructure
==========================

1. Automatic build system and how to build your program

  I will use GNU Make to build my system.

2. Version control (show how you will use it)

  I will use Git to do version control. There will be a master and
  develop branch. Each week I will merge develop branch to master.

3. Testing framework

  I will use Pytest to test my code.

4. Documentation

  I'm not really sure where to publish it, I think I will write it down
  first and then put on a github.io page.

Schedule
========

* Planning phase (6 weeks from 9/19 to 10/31):
* Week 1 (10/31): Read C++ template programming and pybind11 docs.
* Week 2 (11/7): Read C++ template programming and pybind11 docs, try to write a
  little piece of code to understand them well.
* Week 3 (11/14): Implement columnarArray imfrastructure.
* Week 4 (11/21): Implement push_back, erase function.
-- changed topic --
* Week 5 (11/28): Implement Array class and methods.
* Week 6 (12/5): Implement python interface.
* Week 7 (12/12): Integrate python tests. (Tests should be done once
  a new feature is added to the test.)
* Week 8 (12/19): Final debug and preparing report.

References
==========

https://yyc.solvcon.net/en/latest/nsd/schedule/22au_nycu/schedule.html#columnar-array
https://en.cppreference.com/w/cpp/language/templates
https://github.com/Lunarsong/StructureOfArrays
https://arrow.apache.org/docs/format/Columnar.html
https://numpy.org/doc/stable/user/basics.creation.html