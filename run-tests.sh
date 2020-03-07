#!/bin/sh

find ./build -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{} --gtest_list_tests'
find ./build -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{}'
sh projects/01/run-test.sh
sh projects/02/run-test.sh

