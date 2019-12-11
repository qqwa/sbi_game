#!/bin/bash
set -e

mkdir -p build 
cd build 
cmake ../WyvernEngine 
make
cd ..

./build/sbigame/sbi_game