#!/bin/sh

cd `dirname $0`
cd ..

rm -rf test

for i in A B C D E F G H
do
  cp template/template_ucup.cpp src/$i.cpp
done