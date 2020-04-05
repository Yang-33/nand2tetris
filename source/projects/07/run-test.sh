#!/bin/bash

status=0
for f in \
source/projects/07/StackArithmetic/SimpleAdd/SimpleAdd \
source/projects/07/StackArithmetic/StackTest/StackTest \
source/projects/07/MemoryAccess/BasicTest/BasicTest \
source/projects/07/MemoryAccess/PointerTest/PointerTest \
source/projects/07/MemoryAccess/StaticTest/StaticTest
do
  echo $f
  ./build/bin/VMtranslator ${f}.vm;
  sh external/tools/CPUEmulator.sh ${f}.tst;
  if [ $? -ne 0 ]; then
    status=1
  fi
done

exit $status