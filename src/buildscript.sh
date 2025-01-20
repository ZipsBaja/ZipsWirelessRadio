#!/bin/bash
cd ./client/build
cmake ..
make
cd ../../server/build
cmake ..
make
