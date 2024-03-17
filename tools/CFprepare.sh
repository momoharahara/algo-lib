#!/bin/sh

cd `dirname $0`
cd ..

contest_id='1928'
contest_url='https://codeforces.com/contest/'$contest_id'/problem/'
rm -rf test

for i in A B C D E F G
do
  # oj download $contest_url$i -d test/$i/
  cp template/template_cf.cpp src/$i.cpp
done