#! /bin/bash

make submake

cd buildDict

./makeDict

cd ../buildDB

./makeDB

cd ..

make
