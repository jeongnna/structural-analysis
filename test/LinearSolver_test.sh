#!/bin/sh
c++ -std=c++14 -I ../include LinearSolver_test.cpp ../src/Matrix.cpp ../src/LinearSolver.cpp -o LinearSolver_test
./LinearSolver_test
rm LinearSolver_test
