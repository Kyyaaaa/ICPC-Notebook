#!/bin/bash
set -e

name="${1%.cpp}"

g++ -std=c++17 -O2 -pipe "$name.cpp" -o "$name"

# Cap quyen: chmod +x com.sh
# Xem diff: diff -u file1 file2
# Do time chay: time ./main < TestInput
# Gioi han time chay: timeout 2s ./main < TestInput