#!/bin/bash

# Memory.tst needs GUI since it needs input from a keyboard.

status=0
for f in \
source/projects/08/ProgramFlow/BasicLoop/BasicLoop
# source/projects/08/ProgramFlow/FibonacciSeries/FibonacciSeries 
do
  echo $f
  ./build/bin/VMtranslator ${f}.vm;
  sh external/tools/CPUEmulator.sh ${f}.tst;
  if [ $? -ne 0 ]; then
    status=1
  fi
done


## TODO: Add FunctionCalls

exit $status