#!/bin/bash

status=0

./build/bin/JackAnalyzer source/projects/10/ArrayTest
sh external/tools/TextComparer.sh source/projects/10/ArrayTest/Main-y3.xml source/projects/10/ArrayTest/Main.xml
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackAnalyzer source/projects/10/ExpressionLessSquare
sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/Main-y3.xml source/projects/10/ExpressionLessSquare/Main.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/SquareGame-y3.xml source/projects/10/ExpressionLessSquare/SquareGame.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/ExpressionLessSquare/Square-y3.xml source/projects/10/ExpressionLessSquare/Square.xml
if [ $? -ne 0 ]; then
  status=1
fi

./build/bin/JackAnalyzer source/projects/10/Square
sh external/tools/TextComparer.sh source/projects/10/Square/Main-y3.xml source/projects/10/Square/Main.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/Square/SquareGame-y3.xml source/projects/10/Square/SquareGame.xml
if [ $? -ne 0 ]; then
  status=1
fi

sh external/tools/TextComparer.sh source/projects/10/Square/Square-y3.xml source/projects/10/Square/Square.xml
if [ $? -ne 0 ]; then
  status=1
fi


exit $status