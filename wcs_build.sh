#! /bin/bash

make submake

cd dict

./makeDict

cd ../buildDB

./build.sh

./makeDB

cd ..

make
