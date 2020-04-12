#include "CompilationEngine.h"

#include <assert.h>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace nand2tetris {
    CompilationEngine::CompilationEngine(const std::string& outputfile) :
        ofs_(outputfile), filename_(""), labelid_(0), return_labelid_(0), current_function_name_("") {
        auto dot_index = outputfile.find_last_of('.');
        auto last_slash_index = outputfile.find_last_of('/');
        filename_ = outputfile.substr(last_slash_index + 1, dot_index);
    }
    // Inform beginning translation a new vm file.
    void CompilationEngine::setFileName(const std::string& filename) {
        auto dot_index = filename.find_last_of('.');
        auto last_slash_index = filename.find_last_of('/');
        filename_ = filename.substr(last_slash_index + 1, dot_index);
    }

    void CompilationEngine::compileClass() {}
    void CompilationEngine::compileClassVarDec() {}
    void CompilationEngine::compileSubroutine() {}
    void CompilationEngine::compileParameterList() {}
    void CompilationEngine::compileVarDec() {}
    void CompilationEngine::compileStatements() {}
    void CompilationEngine::compileDo() {}
    void CompilationEngine::compileLet() {}
    void CompilationEngine::compileWhile() {}
    void CompilationEngine::compileReturn() {}
    void CompilationEngine::compileIf() {}
    void CompilationEngine::compileExpression() {}
    void CompilationEngine::compileTerm() {}
    void CompilationEngine::compileExpressionList() {}
}  // namespace nand2tetris
