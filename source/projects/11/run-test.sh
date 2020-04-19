#!/bin/bash

status=0

./build/bin/JackCompiler source/projects/11/Seven
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackCompiler source/projects/11/ConvertToBin
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackCompiler source/projects/11/Square
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackCompiler source/projects/11/Average
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackCompiler source/projects/11/Pong
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackCompiler source/projects/11/ComplexArrays
if [ $? -ne 0 ]; then
  status=1
fi

exit $status