#ifndef SOURCE_PROJECT_10_COMPILATIONENGINE_H_
#define SOURCE_PROJECT_10_COMPILATIONENGINE_H_

#include <string>
#include <fstream>
#include "src_Export.h"


namespace nand2tetris {

    class src_EXPORT CompilationEngine {
    public:
        // Prepare for writing on |outputfile|.
        explicit CompilationEngine(const std::string& outputfile);
        // Inform beginning translation a new vm file.
        void setFileName(const std::string& filename);
        // Transform the given arithmetic command into assembly code and write it.

        void compileClass();
        void compileClassVarDec();
        void compileSubroutine();
        void compileParameterList();
        void compileVarDec();
        void compileStatements();
        void compileDo();
        void compileLet();
        void compileWhile();
        void compileReturn();
        void compileIf();
        void compileExpression();
        void compileTerm();
        void compileExpressionList();

    private:
        std::ofstream ofs_;
        // |filename_| is used to create static var.
        std::string filename_;
        int labelid_;
        int return_labelid_;
        std::string current_function_name_;
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_10_COMPILATIONENGINE_H_