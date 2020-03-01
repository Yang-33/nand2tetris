#!/bin/sh

find . -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{} --gtest_list_tests'
find . -name "tests*" -type f -executable | xargs -n 1 -I {} sh -c '{}'
