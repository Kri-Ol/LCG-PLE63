LCG-PLE63
=========

Linear Congruential RNG, 63bits, fast, with logarithmic skip-ahead

    Prerequisites

GNU make, GCC with support of C++11 (4.7.x and 4.8.x shall work).
64bit platform is pretty much a requirement, though X32 might work
as well.

    Build Debug

Type "make", which equivalent to "make CONFIG=Debug". If you make is
of a GNU origin, version 3.80 or 3.81, you'll get executable which
executes all tests except find_perion() one

    Build Release

Type "make CONFIG=Release"
