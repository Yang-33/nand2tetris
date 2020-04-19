#include "VMWriter.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

namespace nand2tetris {

    VMWriter::VMWriter(const std::string& filepath) :ofs_(filepath) {

    }
    std::string segmentString(Segment segment) {
        if (segment == Segment::ARG) {
            return kSegmentArg;
        }
        else if (segment == Segment::CONST) {
            return kSegmentConst;
        }
        else if (segment == Segment::LOCAL) {
            return kSegmentLocal;
        }
        else if (segment == Segment::POINTER) {
            return kSegmentPointer;
        }
        else if (segment == Segment::STATIC) {
            return kSegmentStatic;
        }
        else if (segment == Segment::TEMP) {
            return kSegmentTemp;
        }
        else if (segment == Segment::THAT) {
            return kSegmentThat;
        }
        else if (segment == Segment::THIS) {
            return kSegmentThis;
        }
        else {
            assert(false);
            return "";
        }
    }
    std::string CommandString(Command command) {
        if (command == Command::ADD) {
            return kCommandAdd;
        }
        else if (command == Command::AND) {
            return kCommandAnd;
        }
        else if (command == Command::EQ) {
            return kCommandEq;
        }
        else if (command == Command::GT) {
            return kCommandGt;
        }
        else if (command == Command::LT) {
            return kCommandLt;
        }
        else if (command == Command::NEG) {
            return kCommandNeg;
        }
        else if (command == Command::NOT) {
            return kCommandNot;
        }
        else if (command == Command::OR) {
            return kCommandOr;
        }
        else if (command == Command::SUB) {
            return kCommandSub;
        }
        else {
            assert(false);
            return "";
        }
    }
    void VMWriter::writePush(Segment segment, int index) {
        ofs_ << "push " << segmentString(segment) << " " << index << std::endl;
    }
    void VMWriter::writePop(Segment segment, int index) {
        ofs_ << "pop " << segmentString(segment) << " " << index << std::endl;
    }
    void VMWriter::writeArithmetic(Command command) {
        ofs_ << CommandString(command) << std::endl;
    }
    void VMWriter::writeLabel(const std::string& label) {
        ofs_ << "label " << label << std::endl;
    }
    void VMWriter::writeGoto(const std::string& label) {
        ofs_ << "goto " << label << std::endl;
    }
    void VMWriter::writeIf(const std::string& label) {
        ofs_ << "if-goto " << label << std::endl;
    }
    void VMWriter::writeCall(const std::string& name, int nArgs) {
        ofs_ << "call " << name << " " << nArgs << std::endl;
    }
    void VMWriter::writeFunction(const std::string& name, int nLocals) {
        ofs_ << "function " << name << " " << nLocals << std::endl;
    }
    void VMWriter::writeReturn() {
        ofs_ << "return" << std::endl;
    }
    void VMWriter::close() {
        ofs_.close();
    }

}  // namespace nand2tetris
