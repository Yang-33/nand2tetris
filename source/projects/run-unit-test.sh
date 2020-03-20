#!/bin/bash

## CAUTION: This script isn't for CI.
## This script for HardwareSimulator test. In particular, it's for
## chapter 3.

TESTDIR="source/projects/unittest"
mkdir $TESTDIR

## Collect all file
for f in \
source/projects/01/* \
source/projects/02/* \
source/projects/03/a/* \
source/projects/03/b/* \

do
  cp $f $TESTDIR/`basename $f`
done

## Execute test
status=0
for f in $TESTDIR/*.tst;
do
  echo $f
  sh external/tools/HardwareSimulator.sh $f;
  if [ $? -ne 0 ]; then
    status=1
  fi
done

rm -rf $TESTDIR

exit $status
