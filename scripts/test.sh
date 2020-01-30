#!/bin/bash

if [ -d "test_lidar_cluster/build_test" ]; 
then
	rm -rf test_lidar_cluster/build_test/*
else
	mkdir test_lidar_cluster/build_test
fi

cd test_lidar_cluster/build_test

qmake ..

make

shopt -s extglob
rm -rf !("test_lidar_cluster")

./test_lidar_cluster
