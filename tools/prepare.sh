#!/bin/sh

cd `dirname $0`
cd ..

contest_name='abc469'
problem_name="${contest_name}"
# problem_name='codequeen2026'
contest_url='https://atcoder.jp/contests/'$contest_name'/tasks/'$problem_name'_'
rm -rf test

for i in a b c d e f g
do
  oj download $contest_url$i -d test/$i/
  cp template/template.cpp src/$i.cpp
done
