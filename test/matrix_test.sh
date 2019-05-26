#!/bin/sh
c++ -std=c++14 -I ../include matrix_test.cpp ../src/Matrix.cpp -o matrix_test
./matrix_test
rm matrix_test
