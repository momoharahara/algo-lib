#!/bin/sh

#echo "cd "/Users/momohara/Desktop/dev/atcoder/" && g++ "$1".cpp -std=c++17 -o a -Wall -Wextra -O2 && /Users/momohara/Desktop/dev/atcoder/a;"

g++ code/$1.cpp -std=c++17 -I . -o AAA -Wall -Wextra -O2 && oj test  -c "./AAA" -d test/$1/