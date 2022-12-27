========================
Columnar Array
========================

Basic Information
=================

https://github.com/soulrrrrr/ColumnarArray

Problem to Solve
================

Columnar Array (or dataframe) is useful in data analysis area. Pandas is a famous library
that provides dataframe library for Python users. I want to implement my version of it, and I
hope it helps people dealing with problems that need to use columnar array.

Prospective Users
=================

People originally uses Array of Struct in Python and wants to speed up.
People wants lighter library for using dataframe.

System Architecture
===================

A class template :cpp:class:`!ColumnarArray` is holding the arrays of struct
and the row accessor of it. The arrays of struct inside :cpp:class:`!SimpleArray`
is :cpp:class:`!Vector`. If it is needed, I will implement the bitmap which
mentioned in https://arrow.apache.org/docs/format/Columnar.html.

At the moment I am planning, :cpp:class:`!ColumnarArray` can only deal with 
structs have numeric types only.

API Description
===============

:cpp:class:`!ColumnarArray` will have a constructor, user can pass python class
into the constructor, and it will be the columnar array of the class.

:cpp:class:`!ColumnarArray` will provide *push_back* to add element, *erase* to 
delete element, and all other things that makes it work like std::vector.
e.g. size(), empty(), and others in https://en.cppreference.com/w/cpp/container/vector.

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
* Week 5 (11/28): Implement other useful functions mentioned in API description.
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