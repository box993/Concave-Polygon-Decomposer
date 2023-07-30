#!/bin/bash

# Run polygenerator.py
python3 polygongenerator.py

# Compile and run daa.cpp
g++-11 -o ./src/daa ./src/DAAFinal.cpp
./src/daa

# Run decomppolygen.py
python3 decomppolygen.py

# Run timeplot.py
python3 timeplot.py
