#!/bin/bash

# Memory.tst needs GUI since it needs input from a keyboard.

status=0
for f in \
source/projects/05/Computer*.tst \
source/projects/05/CPU*.tst
do
  echo $f
  sh external/tools/HardwareSimulator.sh $f;
  if [ $? -ne 0 ]; then
    status=1
  fi
done

exit $status