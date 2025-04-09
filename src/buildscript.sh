#!/bin/bash
mkdir -p ./client/build
cd ./client/build
cmake ..
make -j$(nproc)

mkdir -p ../../server/build
cd ../../server/build
cmake ..
make -j$(nproc)
