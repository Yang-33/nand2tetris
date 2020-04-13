#include "CompilationEngine.h"

#include <assert.h>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "JackTokenizer.h"

namespace nand2tetris {
    CompilationEngine::CompilationEngine(const std::string& inputfile, const std::string& outputfile) :
        tokenizer_(inputfile), ofs_(outputfile), space_size_(0) {
    }
    void CompilationEngine::compile() {
        compileClass();
    }
    void CompilationEngine::compileKeyword(const std::vector<std::string>& targets) {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::KEYWORD);
        for (auto target : targets) {
            if (tokenizer_.getKeyWord() == target) {
                write("keyword", tokenizer_.getKeyWord());
                return;
            }
        }
        std::cerr << tokenizer_.getKeyWord() << std::endl;
        assert(false);
    }
    void CompilationEngine::compileSymbol(const std::vector<std::string>& targets) {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::SYMBOL);
        for (auto target : targets) {
            if (tokenizer_.symbol() == target) {
                write("symbol", tokenizer_.symbol());
                return;
            }
        }
        std::cerr << tokenizer_.symbol() << std::endl;
        assert(false);
    }
    void CompilationEngine::compileIdentifier() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::IDENTIFIER);
        write("identifier", tokenizer_.identifier());
    }
    void CompilationEngine::compileIntegerConstant() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::INT_CONST);
        write("integerConstant", std::to_string(tokenizer_.intVal()));
    }
    void CompilationEngine::compileStringConstant() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::STRING_CONST);
        write("stringConstant", tokenizer_.stringVal());
    }

    void CompilationEngine::compileType() {
        if (isNext({ kKeyword_Int, kKeyword_Char,kKeyword_Boolean })) {
            compileKeyword({ kKeyword_Int, kKeyword_Char,kKeyword_Boolean });
        }
        else {
            compileIdentifier();
        }
    }
    void CompilationEngine::compileVarName() {
        compileIdentifier();
    }
    void CompilationEngine::compileSubroutineName() {
        compileIdentifier();
    }
    void CompilationEngine::compileStatement() {

        if (isNextLetStatement()) {
            compileLetStatement();
        }
        else if (isNextIfStatement()) {
            compileIfStatement();
        }
        else if (isNextWhileStatement()) {
            compileWhileStatement();
        }
        else if (isNextDoStatement()) {
            compileDoStatement();
        }
        else if (isNextReturnStatement()) {
            compileReturnStatement();
        }
        else {
            std::cerr << tokenizer_.nextToken() << std::endl;
            assert(false);
        }
    }
    void CompilationEngine::compileSubroutineCall() {
        if (isNextIdentifier() && isNextNext({ "(" })) {
            compileSubroutineName();
            compileSymbol({ "(" });
            compileExpressionList();
            compileSymbol({ ")" });
        }
        else {
            // (className | varName)
            compileIdentifier();
            compileSymbol({ "." });
            compileSubroutineName();
            compileSymbol({ "(" });
            compileExpressionList();
            compileSymbol({ ")" });
        }

    }
    void CompilationEngine::compileOp() {
        compileSymbol({ "+", "-", "*", "/", "&", "|", "<", ">", "=" });
    }
    void CompilationEngine::compileUnaryOp() {
        compileSymbol({ "-", "~" });
    }
    void CompilationEngine::compileKeywordConstant() {
        compileKeyword({ kKeyword_True, kKeyword_False,kKeyword_Null,kKeyword_This });
    }
    bool CompilationEngine::isNextVarDec() {
        return isNext({ kKeyword_Var });
    }
    bool CompilationEngine::isNextIdentifier() {
        return tokenizer_.tokenType(tokenizer_.nextToken()) == TokenType::IDENTIFIER;
    }
    bool CompilationEngine::isNextType() {
        if (isNext({ kKeyword_Int,kKeyword_Char,kKeyword_Boolean })) {
            return true;
        }
        if (isNextIdentifier()) {
            return true;
        }
        return false;
    }
    bool CompilationEngine::isNextClassVarDec() {
        return isNext({ kKeyword_Static, kKeyword_Field });
    }
    bool CompilationEngine::isNextSubroutineDec() {
        return isNext({ kKeyword_Constructor, kKeyword_Function, kKeyword_Method });
    }
    bool CompilationEngine::isNext(const std::vector<std::string>& targets) {
        for (auto target : targets) {
            if (target == tokenizer_.nextToken())return true;
        }
        return false;
    }
    bool CompilationEngine::isNextNext(const std::vector<std::string>& targets) {
        for (auto target : targets) {
            if (target == tokenizer_.nextNextToken())return true;
        }
        return false;
    }
    bool CompilationEngine::isNextStatement() {
        return isNextLetStatement()
            || isNextIfStatement()
            || isNextWhileStatement()
            || isNextDoStatement()
            || isNextReturnStatement();
    }
    bool CompilationEngine::isNextLetStatement() {
        return isNext({ kKeyword_Let });
    }
    bool CompilationEngine::isNextIfStatement() {
        return isNext({ kKeyword_If });
    }
    bool CompilationEngine::isNextWhileStatement() {
        return isNext({ kKeyword_While });
    }
    bool CompilationEngine::isNextDoStatement() {
        return isNext({ kKeyword_Do });
    }
    bool CompilationEngine::isNextReturnStatement() {
        return isNext({ kKeyword_Return });
    }
    bool CompilationEngine::isNextExpression() {
        return isNextTerm();
    }
    bool CompilationEngine::isNextTerm() {
        return isNextIntergerConstant()
            || isNextStringConstant()
            || isNextKeywordConstant()
            || isNextVarName()
            || isNextSubroutineCall()
            || isNext({ "(" })
            || isNextUnaryOp();
    }
    bool CompilationEngine::isNextOp() {
        return isNext({ "+", "-", "*", "/", "&" ,"|", "<", ">" ,"=" });
    }

    void CompilationEngine::compileClass() {
        startTag("class");

        compileKeyword({ kKeyword_Class });
        // className
        compileIdentifier();
        compileSymbol({ "{" });
        while (isNextClassVarDec()) {
            compileClassVarDec();
        }

        while (isNextSubroutineDec()) {
            compileSubroutineDec();
        }

        compileSymbol({ "}" });

        endTag("class");
    }
    void CompilationEngine::compileClassVarDec() {
        startTag("classVarDec");

        compileKeyword({ kKeyword_Static, kKeyword_Field });
        compileType();
        compileVarName();

        while (isNext({ "," })) {
            compileSymbol({ "," });
            compileVarName();
        }

        compileSymbol({ ";" });

        endTag("classVarDec");
    }
    void CompilationEngine::compileSubroutineDec() {
        startTag("subroutineDec");

        compileKeyword({ kKeyword_Constructor, kKeyword_Function, kKeyword_Method });

        if (isNext({ kKeyword_Void })) {
            compileKeyword({ kKeyword_Void });
        }
        else {
            compileType();
        }

        compileSubroutineName();
        compileSymbol({ "(" });
        compileParameterList();
        compileSymbol({ ")" });
        compileSubroutineBody();

        endTag("subroutineDec");
    }
    void CompilationEngine::compileParameterList() {
        startTag("parameterList");

        if (isNextType()) {
            compileType();
            compileVarName();
            while (isNext({ "," })) {
                compileSymbol({ "," });
                compileType();
                compileVarName();
            }
        }

        endTag("parameterList");
    }
    void CompilationEngine::compileVarDec() {
        startTag("varDec");

        compileKeyword({ kKeyword_Var });
        compileType();
        compileVarName();
        while (isNext({ "," })) {
            compileSymbol({ "," });
            compileVarName();
        }
        compileSymbol({ ";" });

        endTag("varDec");
    }
    void CompilationEngine::compileSubroutineBody() {
        startTag("subroutineBody");

        compileSymbol({ "{" });
        while (isNextVarDec()) {
            compileVarDec();
        }
        compileStatements();
        compileSymbol({ "}" });

        endTag("subroutineBody");
    }
    void CompilationEngine::compileStatements() {
        startTag("statements");

        while (isNextStatement()) {
            compileStatement();
        }

        endTag("statements");
    }
    void CompilationEngine::compileDoStatement() {
        startTag("doStatement");

        compileKeyword({ kKeyword_Do });
        compileSubroutineCall();
        compileSymbol({ ";" });

        endTag("doStatement");
    }
    void CompilationEngine::compileLetStatement() {
        startTag("letStatement");

        compileKeyword({ kKeyword_Let });
        compileVarName();
        if (isNext({ "[" })) {
            compileSymbol({ "[" });
            compileExpression();
            compileSymbol({ "]" });
        }
        compileSymbol({ "=" });
        compileExpression();
        compileSymbol({ ";" });

        endTag("letStatement");
    }
    void CompilationEngine::compileWhileStatement() {
        startTag("whileStatement");

        compileKeyword({ kKeyword_While });
        compileSymbol({ "(" });
        compileExpression();
        compileSymbol({ ")" });
        compileSymbol({ "{" });
        compileStatements();
        compileSymbol({ "}" });

        endTag("whileStatement");
    }
    void CompilationEngine::compileReturnStatement() {
        startTag("returnStatement");

        compileKeyword({ kKeyword_Return });
        if (isNextExpression()) {
            compileExpression();
        }
        compileSymbol({ ";" });

        endTag("returnStatement");
    }
    void CompilationEngine::compileIfStatement() {
        startTag("ifStatement");

        compileKeyword({ kKeyword_If });
        compileSymbol({ "(" });
        compileExpression();
        compileSymbol({ ")" });
        compileSymbol({ "{" });
        compileStatements();
        compileSymbol({ "}" });

        if (isNext({ kKeyword_Else })) {
            compileKeyword({ kKeyword_Else });
            compileSymbol({ "{" });
            compileStatements();
            compileSymbol({ "}" });
        }

        endTag("ifStatement");
    }
    void CompilationEngine::compileExpression() {
        startTag("expression");

        compileTerm();
        while (isNextOp()) {
            compileOp();
            compileTerm();
        }

        endTag("expression");
    }
    bool CompilationEngine::isNextIntergerConstant() {
        return tokenizer_.tokenType(tokenizer_.nextToken()) == TokenType::INT_CONST;
    }
    bool CompilationEngine::isNextStringConstant() {
        return tokenizer_.tokenType(tokenizer_.nextToken()) == TokenType::STRING_CONST;
    }
    bool CompilationEngine::isNextKeywordConstant() {
        return isNext({ kKeyword_True, kKeyword_False,kKeyword_Null,kKeyword_This });
    }
    bool CompilationEngine::isNextVarName() {
        return isNextIdentifier();
    }
    bool CompilationEngine::isNextSubroutineCall() {
        return  isNextIdentifier() && isNextNext({ "(", "." });
    }
    bool CompilationEngine::isNextUnaryOp() {
        return isNext({ "-", "~" });
    }

    void CompilationEngine::compileTerm() {
        startTag("term");

        if (isNextIntergerConstant()) {
            compileIntegerConstant();
        }
        else if (isNextStringConstant()) {
            compileStringConstant();
        }
        else if (isNextKeywordConstant()) {
            compileKeywordConstant();
        }
        else if (isNextSubroutineCall()) {
            compileSubroutineCall();
        }
        else if (isNextVarName()) {
            compileVarName();
            if (isNext({ "[" })) {
                compileSymbol({ "[" });
                compileExpression();
                compileSymbol({ "]" });
            }
        }
        else if (isNext({ "(" })) {
            compileSymbol({ "(" });
            compileExpression();
            compileSymbol({ ")" });
        }
        else if (isNextUnaryOp()) {
            compileUnaryOp();
            compileTerm();
        }

        endTag("term");
    }
    void CompilationEngine::compileExpressionList() {
        startTag("expressionList");

        if (isNextExpression()) {
            compileExpression();
            while (isNext({ "," })) {
                compileSymbol({ "," });
                compileExpression();
            }
        }

        endTag("expressionList");
    }
    void CompilationEngine::startTag(const std::string& tag) {
        ofs_ << std::string(2 * space_size_, ' ');
        ofs_ << "<" << tag << ">" << std::endl;
        ++space_size_;
    }
    void CompilationEngine::endTag(const std::string& tag) {
        --space_size_;
        ofs_ << std::string(2 * space_size_, ' ');
        ofs_ << "</" << tag << ">" << std::endl;
    }
    void CompilationEngine::write(const std::string& tag, std::string s) {
        ofs_ << std::string(2 * space_size_, ' ');
        if (s == "<")s = "&lt;";
        if (s == ">")s = "&gt;";
        if (s == "&")s = "&amp;";
        ofs_ << "<" << tag << "> " << s << " </" << tag << ">" << std::endl;
    }

}  // namespace nand2tetris
