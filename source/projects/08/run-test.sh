#!/bin/bash

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

# FunctionCalls/NestedCall
./build/bin/VMtranslator-all source/projects/08/FunctionCalls/NestedCall
sh external/tools/CPUEmulator.sh source/projects/08/FunctionCalls/NestedCall/NestedCall.tst
if [ $? -ne 0 ]; then
  status=1
fi

# FunctionCalls/FibonacciElement
./build/bin/VMtranslator-all source/projects/08/FunctionCalls/FibonacciElement
sh external/tools/CPUEmulator.sh source/projects/08/FunctionCalls/FibonacciElement/FibonacciElement.tst
if [ $? -ne 0 ]; then
  status=1
fi

# StaticsTest
./build/bin/VMtranslator-all source/projects/08/FunctionCalls/StaticsTest
sh external/tools/CPUEmulator.sh source/projects/08/FunctionCalls/StaticsTest/StaticsTest.tst
if [ $? -ne 0 ]; then
  status=1
fi

exit $status