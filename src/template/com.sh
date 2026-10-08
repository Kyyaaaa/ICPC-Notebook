#!/bin/bash
set -e

name="${1%.cpp}"

g++ -std=c++17 -O2 -pipe "$name.cpp" -o "$name"

# Cap quyen: chmod +x com.sh