#!/bin/bash

status=0
for f in source/projects/01/*.tst;
do
  echo $f
  sh external/tools/HardwareSimulator.sh $f;
  if [ $? -ne 0 ]; then
    status=1
  fi
done

exit $status