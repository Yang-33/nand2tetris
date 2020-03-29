# nand2tetris [![Build Status](https://travis-ci.com/Yang-33/nand2tetris.svg?token=yy7PKhspcM5kcKYXsoNB&branch=master)](https://travis-ci.com/Yang-33/nand2tetris)
nand2tetris

Official site: https://www.nand2tetris.org/

Project slide: https://www.nand2tetris.org/course

https://github.com/y-meguro/nand2tetris/tree/master/projects

https://github.com/alexnhan/nand2tetris/tree/master/projects

## Hack specification

A Register  
For data register and address register.
``` c
// for address register 
@516 // A=516
D=M-1
means
D = Memory[516] - 1

@sum // A=17 (sum is located in 17 for instance)
M=0 // initialize
means 
sum = 0;
```

when we use `M`, it refs `A`.

``` c
// for instruction
@35 // A=35 (instruction)
D;JMP
```

D Register  
For only deta value.
