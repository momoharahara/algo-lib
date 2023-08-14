#!/bin/sh

cd `dirname $0`
cd ..

src_file="src/$1.cpp"
bin_file="bin/$1.out"
test_folder="test/$1/"

g++ $src_file -std=gnu++20 -I lib -o $bin_file -Wall -Wextra -O2 && oj test  -c "$bin_file" -d "$test_folder"