#!/bin/sh

g++ src/$1.cpp -std=c++17 -I lib -o bin/$1.out -Wall -Wextra -O2 && /Users/momohara/Desktop/dev/atcoder/bin/$1.out