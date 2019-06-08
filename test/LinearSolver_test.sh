#!/bin/sh
c++ -std=c++14 -I ../include linearsolver_test.cpp ../src/Matrix.cpp ../src/LinearSolver.cpp -o linearsolver_test
./linearsolver_test
rm linearsolver_test
