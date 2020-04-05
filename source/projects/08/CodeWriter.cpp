#include "CodeWriter.h"

#include <assert.h>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "Parser.h"

namespace nand2tetris {
    CodeWriter::CodeWriter(const std::string& outputfile) :
        ofs_(outputfile), filename_(""), labelid_(0), return_labelid_(0), current_function_name_("") {
        auto dot_index = outputfile.find_last_of('.');
        auto last_slash_index = outputfile.find_last_of('/');
        filename_ = outputfile.substr(last_slash_index + 1, dot_index);
    }
    // Inform beginning translation a new vm file.
    void CodeWriter::setFileName(const std::string& filename) {
        auto dot_index = filename.find_last_of('.');
        auto last_slash_index = filename.find_last_of('/');
        filename_ = filename.substr(last_slash_index + 1, dot_index);
    }


    void CodeWriter::writeInit() {
        // SP = 256
        ofs_ << "@256" << std::endl;
        ofs_ << "D=A" << std::endl;
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=D" << std::endl;

        // call Sys.init
        writeCall("Sys.init", 0);
    }

    std::string CodeWriter::createSymbolFromLabel(const std::string& label) {
        if (current_function_name_.size()) {
            return current_function_name_ + "$" + label;
        }
        else {
            return "null$" + label;// ?
        }
    }
    void CodeWriter::writeLabel(const std::string& label) {
        ofs_ << "(" << createSymbolFromLabel(label) << ")" << std::endl;
    }
    void CodeWriter::writeGoto(const std::string& label) {
        // Goto |label|.
        ofs_ << "@" << createSymbolFromLabel(label) << std::endl; // @?func.label
        ofs_ << "0;JMP" << std::endl;
    }
    void CodeWriter::writeIf(const std::string& label) {
        // Look at top of the stack.
        // If True, goto |label|
        writeDecrementSP(); // --SP;
        writeStackTopToDest('D'); // D = *SP;
        ofs_ << "@" << createSymbolFromLabel(label) << std::endl; // @?func.label
        ofs_ << "D;JNE" << std::endl; // D;JNE
    }

    void CodeWriter::writeCall(const std::string& function_name, int num_args) {
        // push return-address
        std::string return_label = createNewReturnLabel();
        ofs_ << "@" << return_label << std::endl;
        ofs_ << "D=A" << std::endl;
        writeStackTopFromComp("D"); // *SP = return-address
        writeIncrementSP(); // ++SP

        // push LCL
        ofs_ << "@LCL" << std::endl;
        ofs_ << "D=M" << std::endl;
        writeStackTopFromComp("D"); // *SP = LCL
        writeIncrementSP(); // ++SP

        // push ARG
        ofs_ << "@ARG" << std::endl;
        ofs_ << "D=M" << std::endl;
        writeStackTopFromComp("D"); // *SP = ARG
        writeIncrementSP(); // ++SP

        // push THIS
        ofs_ << "@THIS" << std::endl;
        ofs_ << "D=M" << std::endl;
        writeStackTopFromComp("D"); // *SP = THIS
        writeIncrementSP(); // ++SP

        // push THAT
        ofs_ << "@THAT" << std::endl;
        ofs_ << "D=M" << std::endl;
        writeStackTopFromComp("D"); // *SP = THAT
        writeIncrementSP(); // ++SP

        // ARG = SP - n - 5
        ofs_ << "@SP" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@5" << std::endl;
        ofs_ << "D=D-A" << std::endl;
        ofs_ << "@" << std::to_string(num_args) << std::endl;
        ofs_ << "D=D-A" << std::endl;
        ofs_ << "@ARG" << std::endl;
        ofs_ << "M=D" << std::endl;

        // LCL = SP
        ofs_ << "@SP" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@LCL" << std::endl;
        ofs_ << "M=D" << std::endl;


        // goto f
        ofs_ << "@" << function_name << std::endl;
        ofs_ << "0;JMP" << std::endl;

        // (return-address)
        ofs_ << "(" + return_label << ")" << std::endl;
    }
    void CodeWriter::writeReturn() {

        // FRAME = LCL
        ofs_ << "@LCL" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=D" << std::endl; // R13 = FRAME = LCL

        // RET = *(FRAME-5)
        ofs_ << "@5" << std::endl;
        ofs_ << "A=D-A" << std::endl; // FRAME-5
        ofs_ << "D=M" << std::endl; // D = M[R13-5]
        ofs_ << "@R14" << std::endl;
        ofs_ << "M=D" << std::endl; // R14 = M[R13-5]

        // *ARG = pop()
        writeDecrementSP(); // --SP
        writeStackTopToDest('D');
        ofs_ << "@ARG" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "M=D" << std::endl;

        // SP = ARG+1
        ofs_ << "D=A" << std::endl;
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=D+1" << std::endl;

        // THAT = *(FRAME-1)
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=M-1" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@THAT" << std::endl;
        ofs_ << "M=D" << std::endl;

        // THIS = *(FRAME-2)
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=M-1" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@THIS" << std::endl;
        ofs_ << "M=D" << std::endl;

        // ARG = *(FRAME-3)
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=M-1" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@ARG" << std::endl;
        ofs_ << "M=D" << std::endl;

        // LCL = *(FRAME-4)
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=M-1" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "D=M" << std::endl;
        ofs_ << "@LCL" << std::endl;
        ofs_ << "M=D" << std::endl;

        // goto RET
        ofs_ << "@R14" << std::endl;
        ofs_ << "A=M" << std::endl; // load
        ofs_ << "0;JMP" << std::endl;
    }
    void CodeWriter::writeFunction(const std::string& function_name, int num_locals) {
        current_function_name_ = function_name;
        ofs_ << "(" << current_function_name_ << ")" << std::endl;
        ofs_ << "D=0" << std::endl;
        for (int i = 0; i < num_locals; ++i) {
            writeStackTopFromComp("D"); // *SP = D
            writeIncrementSP(); // ++SP
        }
    }
    std::string CodeWriter::createNewReturnLabel() {
        return "RETURNLABEL" + std::to_string(return_labelid_++);
    }
    std::string CodeWriter::createNewLabel() {
        return "IFLABEL" + std::to_string(labelid_++);
    }
    void CodeWriter::writeBinaryOparation(const std::string& binary_oparation) {
        writeDecrementSP(); // --SP
        writeStackTopToDest('D'); // D = *SP
        writeDecrementSP(); // --SP
        writeStackTopToDest('A'); // A = *SP
        ofs_ << binary_oparation << std::endl; // D = binary(A,D)
        writeStackTopFromComp("D"); // *SP = D
        writeIncrementSP(); // ++SP
    }
    void CodeWriter::writeUnaryOparation(const std::string& unary_oparation) {
        writeDecrementSP(); // --SP
        writeStackTopToDest('D'); // D = *SP
        ofs_ << unary_oparation << std::endl; // D = [!-] D
        writeStackTopFromComp("D"); // *SP = D
        writeIncrementSP(); // ++SP
    }
    void CodeWriter::writeCompOparation(const std::string& jump_code) {
        writeDecrementSP(); // --SP
        writeStackTopToDest('D'); // D = *SP
        writeDecrementSP(); // --SP
        writeStackTopToDest('A'); // A = *SP
        ofs_ << "D=A-D" << std::endl; // D = A-D
        const std::string labelEQ = createNewLabel();
        const std::string labelNE = createNewLabel();
        ofs_ << "@" << labelEQ << std::endl; // @label1
        ofs_ << "D;" + jump_code << std::endl; // D; |jump_code|
        writeStackTopFromComp("0"); // *SP = 0 : False
        ofs_ << "@" << labelNE << std::endl; // @label2
        ofs_ << "0;JMP" << std::endl; // 0;JMP
        ofs_ << "(" << labelEQ << ")" << std::endl; // label labelEQ
        writeStackTopFromComp("-1"); // *SP = -1 : True
        ofs_ << "(" << labelNE << ")" << std::endl; // label labelNE
        writeIncrementSP(); // ++SP
    }
    // Transform the given arithmetic command into assembly code and write it.
    void CodeWriter::writeArithmetic(const std::string& command) {
        if (command == kARITHMETIC_ADD) {
            writeBinaryOparation("D=A+D");
        }
        else if (command == kARITHMETIC_SUB) {
            writeBinaryOparation("D=A-D");
        }
        else if (command == kARITHMETIC_NEG) {
            writeUnaryOparation("D=-D");
        }
        else if (command == kARITHMETIC_EQ) {
            writeCompOparation("JEQ");
        }
        else if (command == kARITHMETIC_GT) {
            writeCompOparation("JGT");
        }
        else if (command == kARITHMETIC_LT) {
            writeCompOparation("JLT");
        }
        else if (command == kARITHMETIC_AND) {
            writeBinaryOparation("D=A&D");
        }
        else if (command == kARITHMETIC_OR) {
            writeBinaryOparation("D=A|D");
        }
        else if (command == kARITHMETIC_NOT) {
            writeUnaryOparation("D=!D");
        }
    }

    void CodeWriter::writeStackTopFromVirtualSegment(const std::string& segment, int index) {
        ofs_ << "@" << segment << std::endl;
        ofs_ << "D=M" << std::endl; // D=M
        ofs_ << "@" + std::to_string(index) << std::endl; // @index
        ofs_ << "A=D+A" << std::endl; // A = D+A // indexed segment by |index|
        ofs_ << "D=M" << std::endl; // D = M
        writeStackTopFromComp("D"); // *SP = D
        writeIncrementSP(); // ++SP
    }
    void CodeWriter::writeStackTopFromStaticSegment(int base_address, int index) {
        ofs_ << "@" + std::to_string(base_address + index) << std::endl;
        ofs_ << "D=M" << std::endl; // D = M
        writeStackTopFromComp("D"); // *SP = D
        writeIncrementSP(); // ++SP
    }
    void CodeWriter::writeStackTopToVirtualSegment(const std::string& segment, int index) {
        ofs_ << "@" + segment << std::endl;
        ofs_ << "D=M" << std::endl; // D=M
        ofs_ << "@" + std::to_string(index) << std::endl; // @index
        ofs_ << "D=D+A" << std::endl; // D=D+A
        ofs_ << "@R13" << std::endl;
        ofs_ << "M=D" << std::endl; // M=D // save segment[index] to R13
        writeDecrementSP(); // --SP
        writeStackTopToDest('D'); // D = *SP
        ofs_ << "@R13" << std::endl; // M[R13] = D
        ofs_ << "A=M" << std::endl;
        ofs_ << "M=D" << std::endl;
    }
    void CodeWriter::writeStackTopToStaticSegment(int base_address, int index) {
        writeDecrementSP(); // --SP
        writeStackTopToDest('D'); // D = *SP
        ofs_ << "@" << std::to_string(index + base_address) << std::endl; // m[index+k] = D
        ofs_ << "M=D" << std::endl;
    }

    // Transform |C_PUSH| or |C_POP| into assembly code and write it.
    void CodeWriter::writePushPop(
        const std::string& command, const std::string& segment, int index) {
        if (command == kPUSH) {
            if (segment == kCONSTANT) {
                ofs_ << "@" << index << std::endl; // @index
                ofs_ << "D=A" << std::endl; // D = A
                writeStackTopFromComp("D"); // *SP = D
                writeIncrementSP(); // ++SP
            }
            else if (segment == kLOCAL) {
                writeStackTopFromVirtualSegment("LCL", index);
            }
            else if (segment == kARGUMENT) {
                writeStackTopFromVirtualSegment("ARG", index);
            }
            else if (segment == kTHIS) {
                writeStackTopFromVirtualSegment("THIS", index);
            }
            else if (segment == kTHAT) {
                writeStackTopFromVirtualSegment("THAT", index);
            }
            else if (segment == kPOINTER) {
                writeStackTopFromStaticSegment(kPOINTER_BASE_ADDRESS, index);
            }
            else if (segment == kTEMP) {
                writeStackTopFromStaticSegment(kTEMP_BASE_ADDRESS, index);
            }
            else if (segment == kSTATIC) {
                ofs_ << "@" + filename_ + "." + std::to_string(index) << std::endl; // @filename.index
                ofs_ << "D=M" << std::endl; // D = M
                writeStackTopFromComp("D"); // *SP = D
                writeIncrementSP(); // ++SP
            }
            else {
                assert(false);
            }
        }
        else if (command == kPOP) {
            if (segment == kLOCAL) {
                writeStackTopToVirtualSegment("LCL", index);
            }
            else if (segment == kARGUMENT) {
                writeStackTopToVirtualSegment("ARG", index);
            }
            else if (segment == kTHIS) {
                writeStackTopToVirtualSegment("THIS", index);
            }
            else if (segment == kTHAT) {
                writeStackTopToVirtualSegment("THAT", index);
            }
            else if (segment == kPOINTER) {
                writeStackTopToStaticSegment(kPOINTER_BASE_ADDRESS, index);
            }
            else if (segment == kTEMP) {
                writeStackTopToStaticSegment(kTEMP_BASE_ADDRESS, index);
            }
            else if (segment == kSTATIC) {
                writeDecrementSP(); // --SP
                writeStackTopToDest('D'); // D = *SP
                ofs_ << "@" + filename_ + "." + std::to_string(index) << std::endl; // @filename.index
                ofs_ << "M=D" << std::endl;
            }
            else {
                assert(false);
            }
        }
        else {
            assert(false);
        }
    }

    void CodeWriter::writeIncrementSP() {
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=M+1" << std::endl;
    }
    void CodeWriter::writeDecrementSP() {
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=M-1" << std::endl;
    }
    void CodeWriter::writeStackTopToDest(char AorD) {
        ofs_ << "@SP" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << std::string(1, AorD) + "=M" << std::endl;
    }
    void CodeWriter::writeStackTopFromComp(std::string DorNum) {
        ofs_ << "@SP" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "M=" << DorNum << std::endl;
    }
    void CodeWriter::close() {
        ofs_.close();
    }
}  // namespace nand2tetris
