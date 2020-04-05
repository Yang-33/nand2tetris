#!/bin/bash

# Memory.tst needs GUI since it needs input from a keyboard.

status=0
for f in \
source/projects/08/ProgramFlow/BasicLoop/BasicLoop \
source/projects/08/ProgramFlow/FibonacciSeries/FibonacciSeries \
source/projects/08/FunctionCalls/SimpleFunction/SimpleFunction
# source/projects/08/FunctionCalls/NestedCall/NestedCall
do
  echo $f
  ./build/bin/VMtranslator-all ${f}.vm;
  sh external/tools/CPUEmulator.sh ${f}.tst;
  if [ $? -ne 0 ]; then
    status=1
  fi
done


## TODO: Add FunctionCalls

exit $status