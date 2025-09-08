#!/bin/bash

conan install . --build=missing -of ./build -v

cd build

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug

cmake --build .

ninja test

cd ..

