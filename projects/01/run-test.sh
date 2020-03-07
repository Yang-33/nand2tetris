#!/bin/bash

status=0
for f in projects/01/*.tst;
do
  echo $f
  sh tools/HardwareSimulator.sh $f;
  if [ $? -ne 0 ]; then
    status=1
  fi
done

exit $status