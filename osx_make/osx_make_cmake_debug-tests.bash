#!/bin/bash

rm CMakeCache.txt
cmake -DBUILD_FOR_GCOV=1 -DMATH_TESTS_EXECUTABLE=1 -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" ..

echo "make --version"
make --version

echo " "

echo "gcc --version"
gcc --version

