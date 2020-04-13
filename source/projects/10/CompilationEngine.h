#ifndef SOURCE_PROJECT_10_COMPILATIONENGINE_H_
#define SOURCE_PROJECT_10_COMPILATIONENGINE_H_

#include <string>
#include <fstream>
#include <vector>
#include "JackTokenizer.h"
#include "src_Export.h"


namespace nand2tetris {

    class src_EXPORT CompilationEngine {
    public:
        // Prepare for writing on |outputfile|.
        explicit CompilationEngine(const std::string& inputfile, const std::string& outputfile);
        void compile();
    private:
        void compileClass();
        void compileClassVarDec();
        void compileSubroutineDec();
        void compileParameterList();
        void compileVarDec();
        void compileStatements();
        void compileDoStatement();
        void compileLetStatement();
        void compileWhileStatement();
        void compileReturnStatement();
        void compileIfStatement();
        void compileExpression();
        void compileTerm();
        void compileExpressionList();

        void compileSubroutineCall();
        void compileType();
        void compileVarName();
        void compileSubroutineBody();
        void compileSubroutineName();
        void compileStatement();
        void compileOp();
        void compileUnaryOp();
        void compileKeywordConstant();

        bool isNextIdentifier();
        bool isNextOp();
        bool isNextTerm();
        bool isNextExpression();
        bool isNextLetStatement();
        bool isNextIfStatement();
        bool isNextWhileStatement();
        bool isNextDoStatement();
        bool isNextReturnStatement();
        bool isNextStatement();
        bool isNextVarDec();
        bool isNextType();
        bool isNextClassVarDec();
        bool isNextSubroutineDec();
        bool isNextIntergerConstant();
        bool isNextStringConstant();
        bool isNextKeywordConstant();
        bool isNextVarName();
        bool isNextSubroutineCall();
        bool isNextUnaryOp();
        bool isNext(const std::vector<std::string>& targets);
        bool isNextNext(const std::vector<std::string>& targets);

        void compileKeyword(const std::vector<std::string>& targets);
        void compileSymbol(const std::vector<std::string>& targets);
        void compileIdentifier();
        void compileIntegerConstant();
        void compileStringConstant();

        void startTag(const std::string& tag);
        void endTag(const std::string& tag);
        void write(const std::string& tag, std::string s);

        JackTokenizer tokenizer_;
        std::ofstream ofs_;
        int space_size_;
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_10_COMPILATIONENGINE_H_