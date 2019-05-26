#!/bin/sh
c++ -std=c++14 \
-I include \
src/main.cpp \
src/Matrix.cpp \
src/Material.cpp \
src/Load.cpp \
src/Node.cpp \
src/Member.cpp \
src/Frame.cpp \
src/FrameConstructor.cpp \
-o main
