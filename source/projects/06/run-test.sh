#!/bin/bash

# Memory.tst needs GUI since it needs input from a keyboard.

status=0
for f in \
source/projects/06/add/Add \
source/projects/06/max/Max \
source/projects/06/max/MaxL \
source/projects/06/pong/Pong \
source/projects/06/pong/PongL \
source/projects/06/rect/Rect \
source/projects/06/rect/RectL 
do
  echo $f
  sh external/tools/Assembler.sh ${f}.asm;
  ./build/bin/Assembler ${f}.asm;
  diff ${f}.hack ${f}-y3.hack
  if [ $? -ne 0 ]; then
    status=1
  fi
done

exit $status