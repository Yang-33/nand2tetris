#ifndef SOURCE_PROJECT_08_CODEWRITER_H_
#define SOURCE_PROJECT_08_CODEWRITER_H_

#include <string>
#include <fstream>
#include "src_Export.h"


namespace nand2tetris {

    const std::string kCONSTANT = "constant";
    const std::string kLOCAL = "local";
    const std::string kSTATIC = "static";
    const std::string kARGUMENT = "argument";
    const std::string kTHIS = "this";
    const std::string kTHAT = "that";
    const std::string kPOINTER = "pointer";
    const std::string kTEMP = "temp";

    const int kTEMP_BASE_ADDRESS = 5;
    const int kPOINTER_BASE_ADDRESS = 3;

    class src_EXPORT CodeWriter {
    public:
        // Prepare for writing on |outputfile|.
        explicit CodeWriter(const std::string& outputfile);
        // Inform beginning translation a new vm file.
        void setFileName(const std::string& filename);
        // Transform the given arithmetic command into assembly code and write it.
        void writeArithmetic(const std::string& command);
        // Transform |C_PUSH| or |C_POP| into assembly code and write it.
        void writePushPop(const std::string& command, const std::string& segment, int index);

        void writeInit();
        void writeLabel(const std::string& label);
        void writeGoto(const std::string& label);
        void writeIf(const std::string& label);
        void writeCall(const std::string& function_name, int num_args);
        void writeReturn();
        void writeFunction(const std::string& function_name, int num_locals);

        // Close |outputflie|.
        void close();
    private:
        void writeStackTopFromVirtualSegment(const std::string& segment, int index);
        void writeStackTopFromStaticSegment(int base_address, int index);
        void writeStackTopToVirtualSegment(const std::string& segment, int index);
        void writeStackTopToStaticSegment(int base_address, int index);

        void writeBinaryOparation(const std::string& binary_oparation);
        void writeUnaryOparation(const std::string& unary_oparation);
        void writeCompOparation(const std::string& jump_code);

        void writeIncrementSP();
        void writeDecrementSP();

        void writeStackTopToDest(char AorD);
        void writeStackTopFromComp(std::string DorNum);

        std::string createNewLabel();
        std::ofstream ofs_;
        // |filename_| is used to create static var.
        std::string filename_;
        int labelid_;
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_08_CODEWRITER_H_