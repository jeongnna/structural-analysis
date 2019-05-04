#!/bin/sh
c++ -std=c++14 linearsolve_test.cpp Matrix.cpp -o linearsolve_test
./linearsolve_test
rm linearsolve_test
