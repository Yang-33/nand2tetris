#!/bin/sh

find ./build -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{} --gtest_list_tests'
find ./build -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{}'
sh source/projects/01/run-test.sh
sh source/projects/02/run-test.sh
sh source/projects/03/run-test.sh
sh source/projects/04/run-test.sh
sh source/projects/05/run-test.sh
sh source/projects/06/run-test.sh
sh source/projects/07/run-test.sh
sh source/projects/08/run-test.sh
sh source/projects/10/run-test.sh
sh source/projects/11/run-test.sh
# for only local env
# sh source/projects/run-unit-test.sh
