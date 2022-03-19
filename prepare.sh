#!/bin/sh
contest_name='abc243'
contest_url='https://atcoder.jp/contests/'$contest_name'/tasks/'$contest_name'_'
rm -rf test

for i in a b c d e f g h
do
  oj download $contest_url$i -d test/$i/
  # cp template_gcj.cpp code/$i.cpp
  cp template.cpp code/$i.cpp
done
