#!/bin/sh
c++ -std=c++14 -I ../include linearsolve_test.cpp ../src/Matrix.cpp -o linearsolve_test
./linearsolve_test
rm linearsolve_test
