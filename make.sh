#!/bin/sh
c++ -std=c++11 \
-I include \
src/main.cpp \
src/Matrix.cpp \
src/LinearSolver.cpp \
src/Load.cpp \
src/Material.cpp \
src/Node.cpp \
src/Element.cpp \
src/Frame.cpp \
-o main
