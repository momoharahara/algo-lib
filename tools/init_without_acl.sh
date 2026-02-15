#!/bin/sh

cd `dirname $0`
cd ..

rm -rf test

for i in a b c d e f g h
do
  cp template/template_without_acl.cpp src/$i.cpp
done
