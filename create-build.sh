#!/bin/sh

if [ ! -e googletest ]; then
    git submodule init
    git submodule update
fi

DIR=build
CMAKE=cmake

rm -rf $DIR

mkdir $DIR
$CMAKE -B$DIR -H. || exit 1
$CMAKE --build $DIR -- VERBOSE=1 -j 4 || exit 1

