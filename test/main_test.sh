#!/bin/sh
c++ -std=c++14 -I include src/main.cpp src/Matrix.cpp src/Material.cpp src/Load.cpp src/Node.cpp src/Beam.cpp src/Frame.cpp src/FrameConstructor.cpp -o main_test
./main_test
rm main_test
