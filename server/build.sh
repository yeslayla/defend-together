#!/bin/sh

mkdir -p builds

# Remove old version
if [ -f builds/server.out ]; then
    rm builds/server.out
fi

g++ main.cpp -o builds/server.out -lenet -lpthread