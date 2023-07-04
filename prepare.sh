#!/bin/sh
contest_name='abc308'
contest_url='https://atcoder.jp/contests/'$contest_name'/tasks/'$contest_name'_'
rm -rf test

for i in a b c d e f g h
do
  oj download $contest_url$i -d test/$i/
  cp template.cpp src/$i.cpp
  # cp template_gcj.cpp code/$i.cpp
done
