#ifndef SOURCE_PROJECT_11_VMWRITER_H_
#define SOURCE_PROJECT_11_VMWRITER_H_

#include <deque>
#include <string>
#include <fstream>
#include "src_Export.h"


namespace nand2tetris {

    enum class Segment {
        CONST,
        ARG,
        LOCAL,
        STATIC,
        THIS,
        THAT,
        POINTER,
        TEMP
    };
    const std::string kSegmentConst = "const";
    const std::string kSegmentArg = "argument";
    const std::string kSegmentLocal = "local";
    const std::string kSegmentStatic = "static";
    const std::string kSegmentThis = "this";
    const std::string kSegmentThat = "that";
    const std::string kSegmentPointer = "pointer";
    const std::string kSegmentTemp = "temp";

    enum class Command {
        ADD,
        SUB,
        NEG,
        EQ,
        GT,
        LT,
        AND,
        OR,
        NOT
    };
    const std::string kCommandAdd = "add";
    const std::string kCommandSub = "sub";
    const std::string kCommandNeg = "neg";
    const std::string kCommandEq = "eq";
    const std::string kCommandGt = "gt";
    const std::string kCommandLt = "lt";
    const std::string kCommandAnd = "and";
    const std::string kCommandOr = "or";
    const std::string kCommandNot = "not";

    class src_EXPORT VMWriter {
    public:
        explicit VMWriter(const std::string& s);
        void writePush(Segment segment, int index);
        void writePop(Segment segment, int index);
        void writeWrithmetic(Command command);
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeCall(const std::string& name, int nArgs);
        void writeFunction(const std::string& name, int nLocals);
        void writeReturn();
        void close();
    private:
        std::ofstream ofs_;
    };
}  // namespace nand2tetris

#endif // SOURCE_PROJECT_11_VMWRITER_H_
