@echo off
g++ -std=c++17 -O2 -Wl,--stack,536870912 "%~1.cpp" -o "%~1.exe"
