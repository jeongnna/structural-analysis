#!/bin/sh
c++ -std=c++14 beam_test.cpp Matrix.cpp Material.cpp Load.cpp Node.cpp Beam.cpp -o beam_test
./beam_test
rm beam_test
