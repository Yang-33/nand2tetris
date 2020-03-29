#ifndef SOURCE_PROJECT_07_CODEWRITER_H_
#define SOURCE_PROJECT_07_CODEWRITER_H_

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
        // Close |outputflie|.
        void close();
    private:
        void incrementSP();
        void decrementSP();
        void stackTopToDest(char AorD);
        void stackTopFromComp(std::string DorNum);
        std::string create_new_label();
        std::ofstream ofs_;
        // |filename_| is used to create static var. (maybe)
        std::string filename_;
        int labelid_;
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_07_CODEWRITER_H_