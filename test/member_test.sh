#!/bin/sh
c++ -std=c++14 -I ../include member_test.cpp ../src/Matrix.cpp ../src/Material.cpp ../src/Load.cpp ../src/Node.cpp ../src/Member.cpp -o member_test
./member_test
rm member_test
