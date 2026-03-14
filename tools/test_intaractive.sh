#!/bin/sh

set -eu

cd `dirname $0`
cd ..

src_file="src/$1.cpp"
expanded_file="bin/$1.submit.cpp"
bin_file="bin/$1.out"

python3 tools/expand.py "$src_file" -o "$expanded_file"
g++ "$expanded_file" -std=gnu++23 -I lib -o "$bin_file" -Wall -Wextra -O2 && "$bin_file"
