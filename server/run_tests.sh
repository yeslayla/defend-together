#!/bin/sh
mkdir -p builds
g++ tests.cpp -o builds/tests.out -lgtest
./builds/tests.out