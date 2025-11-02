#!/bin/sh

cd `dirname $0`
cd ..

src_file="src/$1.cpp"
bin_file="bin/$1.out"
input_file="tools/in.txt"
output_file="tools/out.txt"


g++ $src_file -std=gnu++23 -I lib -o $bin_file -Wall -Wextra -O2   -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -DDEBUG && $bin_file < $input_file > $output_file