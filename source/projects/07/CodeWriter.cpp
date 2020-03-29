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
        filename_ = outputfile.substr(0, dot_index);
    }
    // Inform beginning translation a new vm file.
    void CodeWriter::setFileName(const std::string& filename) {
        auto dot_index = filename.find_last_of('.');
        filename_ = filename.substr(0, dot_index);
    }

    std::string CodeWriter::create_new_label() {
        return "LABEL" + std::to_string(labelid_++);
    }
    // Transform the given arithmetic command into assembly code and write it.
    void CodeWriter::writeArithmetic(const std::string& command) {
        if (command == kARITHMETIC_ADD) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A+D" << std::endl; // D = binary(A,D)
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_SUB) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A-D" << std::endl; // D = binary(A,D)
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_NEG) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            ofs_ << "D=-D" << std::endl; // D = -D
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_EQ) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A-D" << std::endl; // D = A-D
            const std::string labelEQ = create_new_label();
            const std::string labelNE = create_new_label();
            ofs_ << "@" << labelEQ << std::endl; // @label1
            ofs_ << "D;JEQ" << std::endl; // D;JEQ
            stackTopFromComp("0"); // *SP = 0
            ofs_ << "@" << labelNE << std::endl; // @label2
            ofs_ << "0;JMP" << std::endl; // 0;JMP
            ofs_ << "(" << labelEQ << ")" << std::endl; // label labelEQ
            stackTopFromComp("-1"); // *SP = -1
            ofs_ << "(" << labelNE << ")" << std::endl; // label labelNE
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_GT) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A-D" << std::endl; // D = A-D
            const std::string labelEQ = create_new_label();
            const std::string labelNE = create_new_label();
            ofs_ << "@" << labelEQ << std::endl; // @label1
            ofs_ << "D;JGT" << std::endl; // D;JGT
            stackTopFromComp("0"); // *SP = 0
            ofs_ << "@" << labelNE << std::endl; // @label2
            ofs_ << "0;JMP" << std::endl; // 0;JMP
            ofs_ << "(" << labelEQ << ")" << std::endl; // label labelEQ
            stackTopFromComp("-1"); // *SP = -1
            ofs_ << "(" << labelNE << ")" << std::endl; // label labelNE
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_LT) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A-D" << std::endl; // D = A-D
            const std::string labelEQ = create_new_label();
            const std::string labelNE = create_new_label();
            ofs_ << "@" << labelEQ << std::endl; // @label1
            ofs_ << "D;JLT" << std::endl; // D;JLT
            stackTopFromComp("0"); // *SP = 0
            ofs_ << "@" << labelNE << std::endl; // @label2
            ofs_ << "0;JMP" << std::endl; // 0;JMP
            ofs_ << "(" << labelEQ << ")" << std::endl; // label labelEQ
            stackTopFromComp("-1"); // *SP = -1
            ofs_ << "(" << labelNE << ")" << std::endl; // label labelNE
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_AND) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A&D" << std::endl; // D = binary(A,D)
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_OR) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            decrementSP(); // --SP
            stackTopToDest('A'); // A = *SP
            ofs_ << "D=A|D" << std::endl; // D = binary(A,D)
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
        else if (command == kARITHMETIC_NOT) {
            decrementSP(); // --SP
            stackTopToDest('D'); // D = *SP
            ofs_ << "D=!D" << std::endl; // D = !D
            stackTopFromComp("D"); // *SP = D
            incrementSP(); // ++SP
        }
    }
    // Transform |C_PUSH| or |C_POP| into assembly code and write it.
    void CodeWriter::writePushPop(
        const std::string& command, const std::string& segment, int index) {
        if (command == kPUSH) {
            if (segment == kCONSTANT) {
                ofs_ << "@" << index << std::endl; // @index
                ofs_ << "D=A" << std::endl; // D = A
                stackTopFromComp("D"); // *SP = D
                incrementSP(); // ++SP
            }
        }
        else if (command == kPOP) {

        }
        else {
            assert(false);
        }
    }

    void CodeWriter::incrementSP() {
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=M+1" << std::endl;
    }
    void CodeWriter::decrementSP() {
        ofs_ << "@SP" << std::endl;
        ofs_ << "M=M-1" << std::endl;
    }
    void CodeWriter::stackTopToDest(char AorD) {
        ofs_ << "@SP" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << std::string(1, AorD) + "=M" << std::endl;
    }
    void CodeWriter::stackTopFromComp(std::string DorNum) {
        ofs_ << "@SP" << std::endl;
        ofs_ << "A=M" << std::endl;
        ofs_ << "M=" << DorNum << std::endl;
    }
    void CodeWriter::close() {
        ofs_.close();
    }
}  // namespace nand2tetris
