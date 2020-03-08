#!/bin/bash

sh external/tools/Assembler.sh source/projects/04/mult/Mult.asm
sh external/tools/CPUEmulator.sh source/projects/04/mult/Mult.tst

sh external/tools/Assembler.sh source/projects/04/fill/Fill.asm
sh external/tools/CPUEmulator.sh source/projects/04/fill/FillAutomatic.tst
