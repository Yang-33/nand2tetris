#!/bin/bash

status=0

./build/bin/JackAnalyzer source/projects/10/ArrayTest
sh external/tools/TextComparer.sh source/projects/10/ArrayTest/MainT-y3.xml source/projects/10/ArrayTest/MainT.xml
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackAnalyzer source/projects/10/ExpressionLessSquare
sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/MainT-y3.xml source/projects/10/ExpressionLessSquare/MainT.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/SquareGameT-y3.xml source/projects/10/ExpressionLessSquare/SquareGameT.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/SquareT-y3.xml source/projects/10/ExpressionLessSquare/SquareT.xml
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackAnalyzer source/projects/10/Square
sh external/tools/TextComparer.sh source/projects/10/Square/MainT-y3.xml source/projects/10/Square/MainT.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/Square/SquareGameT-y3.xml source/projects/10/Square/SquareGameT.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/Square/SquareT-y3.xml source/projects/10/Square/SquareT.xml
if [ $? -ne 0 ]; then
  status=1
fi


exit $status