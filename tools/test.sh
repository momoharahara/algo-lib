#!/bin/sh

set -eu

cd `dirname $0`
cd ..

src_file="src/$1.cpp"
expanded_file="bin/$1.submit.cpp"
bin_file="bin/$1.out"
test_folder="test/$1/"

python3 tools/expand.py "$src_file" -o "$expanded_file"
g++ "$expanded_file" -std=gnu++23 -I lib -o "$bin_file" -Wall -Wextra -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -DDEBUG && oj test -c "$bin_file" -d "$test_folder"
pbcopy < "$expanded_file"

# g++ $src_file -std=gnu++23 -I lib -o $bin_file -Wall -Wextra -O2 && oj test  -c "$bin_file" -d "$test_folder"
