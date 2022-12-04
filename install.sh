#!/bin/bash

cmake -S . -B bin
make -C bin
cmake --install bin