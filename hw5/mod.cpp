#include <atomic>
#include <iostream>

#include "matrix.hpp"
#include "_matrix.hpp"

#ifdef __GNUG__
#  define PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))