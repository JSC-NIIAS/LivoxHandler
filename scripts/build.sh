#!/bin/bash

scripts/update_submodules.sh

if [ -d "build" ]; 
then
	rm -rf build/*
else
	mkdir build
fi

sed -i 's/DEFINES += GUI/DEFINES -= GUI/g' lidar_livox.pro

cd build

qmake ..

make

shopt -s extglob
rm -rf !("lidar_livox")
