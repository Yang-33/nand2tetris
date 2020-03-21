#include <iostream>
#include <assert.h>

#include "Code.h"
#include "Parser.h"
#include "SymbolTable.h"

int main(int argc, char* argv[]) {
    std::cout << "Assembler code called!" << std::endl;
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    nand2tetris::Parser parser(argv[1]);

    // TODO(yutakasai): impl 6.3.5

    return 0;
}