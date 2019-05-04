#!/bin/sh
c++ -std=c++14 matrix_test.cpp Matrix.cpp -o matrix_test
./matrix_test
rm matrix_test
