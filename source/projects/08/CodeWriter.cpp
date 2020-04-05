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
        ofs_(outputfile), filename_(""), labelid_(0) {
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


    void writeInit() {
        // TODO: impl it.
    }
    void writeLabel(const std::string& label) {
        // TODO: impl it.
    }
    void writeGoto(const std::string& label) {
        // TODO: impl it.
    }
    void writeIf(const std::string& label) {
        // TODO: impl it.
    }
    void writeCall(const std::string& function_name, int num_args) {
        // TODO: impl it.
    }
    void writeReturn() {
        // TODO: impl it.
    }
    void writeFunction(const std::string& function_name, int num_locals) {
        // TODO: impl it.
    }

    std::string CodeWriter::createNewLabel() {
        return "LABEL" + std::to_string(labelid_++);
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
        writeStackTopFromComp("0"); // *SP = 0
        ofs_ << "@" << labelNE << std::endl; // @label2
        ofs_ << "0;JMP" << std::endl; // 0;JMP
        ofs_ << "(" << labelEQ << ")" << std::endl; // label labelEQ
        writeStackTopFromComp("-1"); // *SP = -1
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
                ofs_ << "@" + filename_ + std::to_string(index) << std::endl; // @filename.index
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
                ofs_ << "@" + filename_ + std::to_string(index) << std::endl; // @filename.index
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
