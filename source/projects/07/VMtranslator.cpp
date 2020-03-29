#include <assert.h>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

#include "Parser.h"
#include "CodeWriter.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    std::string infilepath = argv[1];
    std::string outfilepath; {
        auto dot_index = infilepath.find_last_of('.');
        outfilepath = infilepath.substr(0, dot_index) + ".asm";
    }

    // TODO! if infilepath is directory, rec
    nand2tetris::CodeWriter writer(outfilepath);
    nand2tetris::Parser parser(infilepath);


    while (parser.hasMoreCommands()) {
        parser.advance();
        nand2tetris::VM_COMMAND_TYPE current_command_type = parser.commandType();
        if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_ARITHMETIC) {
            writer.writeArithmetic(parser.arg1());
        }
        else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_PUSH) {
            writer.writePushPop(nand2tetris::kPUSH, parser.arg1(), parser.arg2());
        }
        else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_POP) {
            writer.writePushPop(nand2tetris::kPOP, parser.arg1(), parser.arg2());
        }
        else {
            assert(false);
        }
    }



    std::cout << "VMtranslator assembled " << infilepath << " into " << outfilepath << std::endl;

    return 0;
}