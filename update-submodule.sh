#!/bin/sh

git submodule init
git submodule update
git submodule foreach git reset --hard
git submodule foreach git checkout master
git submodule foreach git pull origin master

