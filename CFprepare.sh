#!/bin/sh
contest_id='1672'
contest_url='https://codeforces.com/contest/'$contest_id'/problem/'
rm -rf test

for i in A B C D E F G
do
  oj download $contest_url$i -d test/$i/
  cp template_cf.cpp src/$i.cpp
done