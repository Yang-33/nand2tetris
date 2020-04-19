#ifndef SOURCE_PROJECT_10_COMPILATIONENGINE_H_
#define SOURCE_PROJECT_10_COMPILATIONENGINE_H_

#include <string>
#include <fstream>
#include <vector>
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"
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
        int compileVarDec();
        void compileStatements();
        void compileDoStatement();
        void compileLetStatement();
        void compileWhileStatement();
        void compileReturnStatement();
        void compileIfStatement();
        void compileExpression();
        void compileTerm();
        int compileExpressionList();

        void compileSubroutineCall();
        std::string compileType();
        std::string compileVarName(bool declaration, const std::string type, SymbolType kind, bool let, bool call);
        void compileSubroutineBody(const std::string& subroutine_name, const std::string& keyword);
        std::string compileSubroutineName();
        void compileStatement();
        std::string compileOp();
        std::string compileUnaryOp();
        void compileKeywordConstant();
        std::string compileKeyword(const std::vector<std::string>& targets);
        std::string compileSymbol(const std::vector<std::string>& targets);
        std::string compileIdentifier();
        int compileIntegerConstant();
        void compileStringConstant();

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

        void startTag(const std::string& tag);
        void endTag(const std::string& tag);
        void write(const std::string& tag, std::string s);

        std::string create_label();

        JackTokenizer tokenizer_;
        SymbolTable symbol_table_;
        VMWriter vmwriter_;
        std::ofstream ofs_;
        int space_size_;
        std::string compiled_class_name_;
        int label_count_;
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_10_COMPILATIONENGINE_H_