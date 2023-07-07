#!/bin/sh

# c++14でコンパイル実行するスクリプト

cd `dirname $0`
cd ..

src_file="src/$1.cpp"
bin_file="bin/$1.out"

g++ $src_file -I lib -o $bin_file -Wall -Wextra -O2 && $bin_file