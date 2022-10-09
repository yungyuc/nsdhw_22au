cmake_minimum_required(VERSION 3.16.3)
project(pybind_vid)
add_subdirectory(pybind11)
pybind11_add_module(_vector q2.cpp)