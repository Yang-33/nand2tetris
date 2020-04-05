#include <assert.h>
#include <cctype>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <string>

#include "Parser.h"
#include "CodeWriter.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    std::string infilepath = argv[1];
    bool is_single_file = true;
    std::string outfilepath;
    std::string path;
    {
        auto last_slash_index = infilepath.find_last_of('/');
        assert(last_slash_index != std::string::npos);
        path = infilepath.substr(0, last_slash_index + 1);
        outfilepath = infilepath.substr(last_slash_index + 1); // /NAME?
        auto dot_index = outfilepath.find_last_of('.');
        if (dot_index == std::string::npos) {
            is_single_file = false;
            path = infilepath + "/";
        }
        outfilepath = path + outfilepath.substr(0, dot_index) + ".asm";
    }

    nand2tetris::CodeWriter writer(outfilepath);

    if (!is_single_file) { // for passing non-initialize test
        writer.writeInit();
    }

    glob_t glob_result;
    std::string target = path + "*.vm";
    glob(target.c_str(), GLOB_TILDE, NULL, &glob_result);
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        std::string filepath = glob_result.gl_pathv[i];
        nand2tetris::Parser parser(filepath);
        writer.setFileName(filepath);

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
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_LABEL) {
                writer.writeLabel(parser.arg1());
            }
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_GOTO) {
                writer.writeGoto(parser.arg1());
            }
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_IF) {
                writer.writeIf(parser.arg1());
            }
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_FUNCTION) {
                writer.writeFunction(parser.arg1(), parser.arg2());
            }
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_RETURN) {
                writer.writeReturn();
            }
            else if (current_command_type == nand2tetris::VM_COMMAND_TYPE::C_CALL) {
                writer.writeCall(parser.arg1(), parser.arg2());
            }
            else {
                assert(false);
            }
        }
    }

    std::cerr << "VMtranslator-all assembled " << infilepath << " into " << outfilepath << std::endl;

    return 0;
}
