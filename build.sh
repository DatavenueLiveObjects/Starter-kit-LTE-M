#!/bin/bash

echo 'clean'
make clean

echo 'check config'

cfglegato

echo 'build'
make wp77xx

build_result=$?

if [ $build_result -eq 0 ]; then
	echo 'run'
	update OrangeStarterKit.wp77xx.update 192.168.2.2
else
	echo 'build failed'
fi

