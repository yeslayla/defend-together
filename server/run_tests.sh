#!/bin/sh
mkdir -p builds
g++ tests.cpp -o builds/tests.out -lgtest
if [ $? = 0 ]
then
    ./builds/tests.out
else
    exit -1
fi