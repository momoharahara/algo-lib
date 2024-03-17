#!/bin/sh

cd `dirname $0`
cd ..

source venv/bin/activate

contest_name='abc345'
contest_url='https://atcoder.jp/contests/'$contest_name'/tasks/'$contest_name'_'
rm -rf test

for i in a b c d e f g
do
  oj download $contest_url$i -d test/$i/
  cp template/template.cpp src/$i.cpp
done
