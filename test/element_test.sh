#!/bin/sh
c++ -std=c++14 -I ../include element_test.cpp ../src/Matrix.cpp ../src/Material.cpp ../src/Load.cpp ../src/Node.cpp ../src/Member.cpp -o element_test
./element_test
rm element_test
