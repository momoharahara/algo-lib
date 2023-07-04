#!/bin/sh

#echo "cd "/Users/momohara/Desktop/dev/atcoder/" && g++ "$1".cpp -std=c++17 -o a -Wall -Wextra -O2 && /Users/momohara/Desktop/dev/atcoder/a;"

g++ src/$1.cpp -std=c++17 -I lib -o bin/$1.out -Wall -Wextra -O2 && oj test  -c "./bin/$1.out" -d test/$1/