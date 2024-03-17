#!/bin/sh

cd `dirname $0`
cd ..

source venv/bin/activate

rm -rf test

for i in a b c d e f g
do
  cp template/template.cpp src/$i.cpp
done
