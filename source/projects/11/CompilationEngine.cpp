#include "CompilationEngine.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>
#include "JackTokenizer.h"

namespace nand2tetris {
    CompilationEngine::CompilationEngine(
        const std::string& inputfile, const std::string& outputfile)
        :tokenizer_(inputfile), symbol_table_(),
        vmwriter_(outputfile), ofs_(outputfile + ".xml"), space_size_(0),
        compiled_class_name_(""), label_count_(0) {
    }
    // OK
    void CompilationEngine::compile() {
        compileClass();
    }

    // OK
    std::string CompilationEngine::compileKeyword(const std::vector<std::string>& targets) {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::KEYWORD);
        for (auto target : targets) {
            if (tokenizer_.getKeyWord() == target) {
                write("keyword", tokenizer_.getKeyWord());
                return tokenizer_.getKeyWord();
            }
        }
        std::cerr << tokenizer_.getKeyWord() << std::endl;
        assert(false);
        return "";
    }

    // OK
    std::string CompilationEngine::compileSymbol(const std::vector<std::string>& targets) {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::SYMBOL);
        for (auto target : targets) {
            if (tokenizer_.symbol() == target) {
                write("symbol", tokenizer_.symbol());
                return tokenizer_.symbol();
            }
        }
        std::cerr << tokenizer_.symbol() << std::endl;
        assert(false);
        return "";
    }

    // OK
    std::string CompilationEngine::compileIdentifier() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::IDENTIFIER);
        write("identifier", tokenizer_.identifier());
        return tokenizer_.identifier();
    }

    // OK
    int CompilationEngine::compileIntegerConstant() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::INT_CONST);
        write("integerConstant", std::to_string(tokenizer_.intVal()));
        return tokenizer_.intVal();
    }

    // OK
    void CompilationEngine::compileStringConstant() {
        assert(tokenizer_.hasMoreTokens());
        tokenizer_.advance();
        assert(tokenizer_.tokenType() == TokenType::STRING_CONST);
        const std::string s = tokenizer_.stringVal();

        write("stringConstant", s);
        vmwriter_.writePush(Segment::CONST, (int)s.size());
        vmwriter_.writeCall("String.new", 1);
        for (auto c : s) {
            vmwriter_.writePush(Segment::CONST, c);
            vmwriter_.writeCall("String.appendChar", 2);
        }
    }

    // OK
    //   int | char | boolean
    // | classname
    std::string CompilationEngine::compileType() {
        if (isNext({ kKeyword_Int, kKeyword_Char,kKeyword_Boolean })) {
            return compileKeyword({ kKeyword_Int, kKeyword_Char,kKeyword_Boolean });
        }
        else {
            // class identifier
            return compileIdentifier();
        }
    }

    // OK
    std::string CompilationEngine::compileVarName(
        bool declaration = false,
        const std::string type = "",
        SymbolType kind = SymbolType::NONE,
        bool let = false,
        bool call = false) {

        std::string token = compileIdentifier();
        if (declaration) {
            symbol_table_.define(token, type, kind);
        }
        else if (let || call) {
            ;
        }
        else {
            kind = symbol_table_.kindOf(token);
            int index = symbol_table_.indexOf(token);
            if (kind == SymbolType::ARG) {
                vmwriter_.writePush(Segment::ARG, index);
            }
            else if (kind == SymbolType::FIELD) {
                vmwriter_.writePush(Segment::THIS, index);
            }
            else if (kind == SymbolType::STATIC) {
                vmwriter_.writePush(Segment::STATIC, index);
            }
            else if (kind == SymbolType::VAR) {
                vmwriter_.writePush(Segment::LOCAL, index);
            }
        }

        return token;
    }

    // OK
    std::string CompilationEngine::compileSubroutineName() {
        return compileIdentifier();
    }

    // OK
    //   letstatement
    // | ifstatement
    // | whilestatement
    // | dostatement
    // | returnstatement
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

    // OK
    //   subroutineName '(' expressionList ')'
    // | (classname | varname) '.' suborutineName '(' expressionList ')'
    void CompilationEngine::compileSubroutineCall() {
        if (isNextIdentifier() && isNextNext({ "(" })) {
            const std::string subroutine_name = compileSubroutineName();
            compileSymbol({ "(" });
            vmwriter_.writePush(Segment::POINTER, 0);
            int arg_count = compileExpressionList();
            compileSymbol({ ")" });
            vmwriter_.writeCall(compiled_class_name_ + "." + subroutine_name, arg_count + 1);
        }
        else {
            if (symbol_table_.kindOf(tokenizer_.nextToken()) != SymbolType::NONE) {
                const SymbolType var_name_kind = symbol_table_.kindOf(tokenizer_.nextToken());
                const std::string var_name = compileVarName(false, "", var_name_kind, false, true);
                compileSymbol({ "." });

                const std::string subroutine_name = compileSubroutineName();
                int index = symbol_table_.indexOf(var_name);
                if (var_name_kind == SymbolType::ARG) {
                    vmwriter_.writePush(Segment::ARG, index);
                }
                else if (var_name_kind == SymbolType::FIELD) {
                    vmwriter_.writePush(Segment::THIS, index);
                }
                else if (var_name_kind == SymbolType::STATIC) {
                    vmwriter_.writePush(Segment::STATIC, index);
                }
                else if (var_name_kind == SymbolType::VAR) {
                    vmwriter_.writePush(Segment::LOCAL, index);
                }

                compileSymbol({ "(" });
                int arg_count = compileExpressionList();
                compileSymbol({ ")" });
                vmwriter_.writeCall(symbol_table_.typeOf(var_name) + "." + subroutine_name, arg_count + 1);
            }
            else {
                const std::string class_name = compileIdentifier();
                compileSymbol({ "." });
                const std::string subroutine_name = compileSubroutineName();
                compileSymbol({ "(" });
                int arg_count = compileExpressionList();
                compileSymbol({ ")" });
                vmwriter_.writeCall(class_name + "." + subroutine_name, arg_count);
            }
        }
    }

    // OK
    // Return op
    std::string CompilationEngine::compileOp() {
        return compileSymbol({ "+", "-", "*", "/", "&", "|", "<", ">", "=" });
    }

    // OK
    // Return unary op
    std::string CompilationEngine::compileUnaryOp() {
        return compileSymbol({ "-", "~" });
    }

    // OK
    void CompilationEngine::compileKeywordConstant() {
        const std::string keyword = compileKeyword({ kKeyword_True, kKeyword_False,kKeyword_Null,kKeyword_This });
        if (keyword == kKeyword_True) {
            vmwriter_.writePush(Segment::CONST, 0);
            vmwriter_.writeArithmetic(Command::NOT);
        }
        else if (keyword == kKeyword_False) {
            vmwriter_.writePush(Segment::CONST, 0);
        }
        else if (keyword == kKeyword_Null) {
            vmwriter_.writePush(Segment::CONST, 0);
        }
        else if (keyword == kKeyword_This) {
            vmwriter_.writePush(Segment::POINTER, 0);
        }
    }

    // OK
    // class { (type) varnameDec *, (func) suborutineDec * }
    void CompilationEngine::compileClass() {
        startTag("class");

        compileKeyword({ kKeyword_Class });
        compiled_class_name_ = compileIdentifier();
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

    // OK
    // (static | field) type name *;
    void CompilationEngine::compileClassVarDec() {
        startTag("classVarDec");

        const std::string keyword = compileKeyword({ kKeyword_Static, kKeyword_Field });
        SymbolType kind; {
            if (keyword == kKeyword_Static)kind = SymbolType::STATIC;
            else if (keyword == kKeyword_Field)kind = SymbolType::FIELD;
            else {
                assert(false);
            }
        }
        const std::string type = compileType();
        compileVarName(true, type, kind);

        while (isNext({ "," })) {
            compileSymbol({ "," });
            compileVarName(true, type, kind);
        }

        compileSymbol({ ";" });

        endTag("classVarDec");
    }

    // OK
    // (constructor | function | method)
    // (void | type) subroutineName '(' parameterList ')'
    // subroutineBody
    void CompilationEngine::compileSubroutineDec() {
        symbol_table_.startSubroutine();

        startTag("subroutineDec");

        const std::string keyword = compileKeyword({ kKeyword_Constructor, kKeyword_Function, kKeyword_Method });
        if (keyword == kKeyword_Method) {
            symbol_table_.define("this", compiled_class_name_, SymbolType::ARG);
        }

        // subroutine type : void or other type
        if (isNext({ kKeyword_Void })) {
            compileKeyword({ kKeyword_Void });
        }
        else {
            compileType();
        }


        const std::string subroutine_name = compileSubroutineName();
        compileSymbol({ "(" });
        compileParameterList();
        compileSymbol({ ")" });
        compileSubroutineBody(subroutine_name, keyword);

        endTag("subroutineDec");
    }

    // OK
    // ((type varName) (',' type varName)*)?
    void CompilationEngine::compileParameterList() {
        startTag("parameterList");

        if (isNextType()) {
            const std::string type = compileType();
            compileVarName(true, type, SymbolType::ARG);
            while (isNext({ "," })) {
                compileSymbol({ "," });
                const std::string type_n = compileType();
                compileVarName(true, type_n, SymbolType::ARG);
            }
        }

        endTag("parameterList");
    }

    // OK
    // 'var' type varName (',' varName)* ';'
    int CompilationEngine::compileVarDec() {
        startTag("varDec");
        int var_dec_count = 0;
        compileKeyword({ kKeyword_Var });
        const std::string type = compileType();
        compileVarName(true, type, SymbolType::VAR);
        ++var_dec_count;
        while (isNext({ "," })) {
            compileSymbol({ "," });
            compileVarName(true, type, SymbolType::VAR);
            ++var_dec_count;
        }
        compileSymbol({ ";" });

        endTag("varDec");
        return var_dec_count;
    }

    // OK
    // '{' varDec* statements '}'
    void CompilationEngine::compileSubroutineBody(const std::string& subroutine_name, const std::string& keyword) {
        startTag("subroutineBody");

        compileSymbol({ "{" });
        int local_num = 0;
        while (isNextVarDec()) {
            local_num += compileVarDec();
        }

        // Register CLASS.SOBROUTINE_NAME [argnum]
        vmwriter_.writeFunction(compiled_class_name_ + "." + subroutine_name, local_num);

        if (keyword == kKeyword_Method) {
            vmwriter_.writePush(Segment::ARG, 0);
            vmwriter_.writePop(Segment::POINTER, 0);
        }
        else if (keyword == kKeyword_Constructor) {
            vmwriter_.writePush(Segment::CONST, symbol_table_.varCount(SymbolType::FIELD));
            vmwriter_.writeCall("Memory.alloc", 1);
            vmwriter_.writePop(Segment::POINTER, 0);
        }
        else if (keyword == kKeyword_Function) {
            ;
        }

        compileStatements();
        compileSymbol({ "}" });

        endTag("subroutineBody");
    }

    // OK
    // statement*
    void CompilationEngine::compileStatements() {
        startTag("statements");

        while (isNextStatement()) {
            compileStatement();
        }

        endTag("statements");
    }

    // OK
    // 'do' subroutineCall ';'
    void CompilationEngine::compileDoStatement() {
        startTag("doStatement");

        compileKeyword({ kKeyword_Do });
        compileSubroutineCall();
        compileSymbol({ ";" });
        // Discard return value of subroutine call
        vmwriter_.writePop(Segment::TEMP, 0);
        endTag("doStatement");
    }

    // OK
    // 'let' varName ('[' expression ']')? '=' expression ';'
    void CompilationEngine::compileLetStatement() {
        startTag("letStatement");

        compileKeyword({ kKeyword_Let });
        const SymbolType let_var_name_kind = symbol_table_.kindOf(tokenizer_.nextToken());
        const std::string let_var_name = compileVarName(false, "", SymbolType::NONE, true);
        int index = symbol_table_.indexOf(let_var_name);
        if (isNext({ "[" })) {

            compileSymbol({ "[" });
            compileExpression();
            compileSymbol({ "]" });

            if (let_var_name_kind == SymbolType::ARG) {
                vmwriter_.writePush(Segment::ARG, index);
            }
            else if (let_var_name_kind == SymbolType::FIELD) {
                vmwriter_.writePush(Segment::THIS, index);
            }
            else if (let_var_name_kind == SymbolType::STATIC) {
                vmwriter_.writePush(Segment::STATIC, index);
            }
            else if (let_var_name_kind == SymbolType::VAR) {
                vmwriter_.writePush(Segment::LOCAL, index);
            }

            vmwriter_.writeArithmetic(Command::ADD);

            compileSymbol({ "=" });
            compileExpression();
            compileSymbol({ ";" });

            vmwriter_.writePop(Segment::TEMP, 0); // value
            vmwriter_.writePop(Segment::POINTER, 1);
            vmwriter_.writePush(Segment::TEMP, 0); // *TEMP=value
            vmwriter_.writePop(Segment::THAT, 0); // v[ex] = *TEMP
        }
        else {
            compileSymbol({ "=" });
            compileExpression();
            compileSymbol({ ";" });
            if (let_var_name_kind == SymbolType::ARG) {
                vmwriter_.writePop(Segment::ARG, index);
            }
            else if (let_var_name_kind == SymbolType::FIELD) {
                vmwriter_.writePop(Segment::THIS, index);
            }
            else if (let_var_name_kind == SymbolType::STATIC) {
                vmwriter_.writePop(Segment::STATIC, index);
            }
            else if (let_var_name_kind == SymbolType::VAR) {
                vmwriter_.writePop(Segment::LOCAL, index);
            }
        }

        endTag("letStatement");
    }

    // OK
    // 'while' '(' expression ')'
    // '{' statements '}'
    void CompilationEngine::compileWhileStatement() {
        startTag("whileStatement");

        const std::string while_head_label = create_label();
        const std::string while_end_label = create_label();

        compileKeyword({ kKeyword_While });
        compileSymbol({ "(" });
        vmwriter_.writeLabel(while_head_label);
        compileExpression();
        vmwriter_.writeArithmetic(Command::NOT);
        vmwriter_.writeIf(while_end_label);
        compileSymbol({ ")" });

        compileSymbol({ "{" });
        compileStatements();
        compileSymbol({ "}" });
        vmwriter_.writeGoto(while_head_label);

        vmwriter_.writeLabel(while_end_label);

        endTag("whileStatement");
    }

    // OK
    // 'return' expression? ';'
    void CompilationEngine::compileReturnStatement() {
        startTag("returnStatement");

        compileKeyword({ kKeyword_Return });
        if (isNextExpression()) {
            compileExpression();
        }
        else {
            vmwriter_.writePush(Segment::CONST, 0);
        }
        compileSymbol({ ";" });
        vmwriter_.writeReturn();

        endTag("returnStatement");
    }

    // OK
    // 'if' '(' expression ')'
    // '{' statements '}'
    // ( 'else' '{' statements '}' )?
    void CompilationEngine::compileIfStatement() {
        startTag("ifStatement");

        compileKeyword({ kKeyword_If });
        compileSymbol({ "(" });
        compileExpression();
        compileSymbol({ ")" });
        vmwriter_.writeArithmetic(Command::NOT);

        const std::string else_label = create_label();
        const std::string if_end_label = create_label();

        vmwriter_.writeIf(else_label);

        compileSymbol({ "{" });
        compileStatements();
        compileSymbol({ "}" });
        vmwriter_.writeGoto(if_end_label);

        vmwriter_.writeLabel(else_label);

        if (isNext({ kKeyword_Else })) {
            compileKeyword({ kKeyword_Else });
            compileSymbol({ "{" });
            compileStatements();
            compileSymbol({ "}" });
        }

        vmwriter_.writeLabel(if_end_label);

        endTag("ifStatement");
    }

    // OK
    // term (op term)*
    void CompilationEngine::compileExpression() {
        startTag("expression");

        compileTerm();
        while (isNextOp()) {
            const std::string op = compileOp();
            compileTerm();

            // Eval in post order
            if (op == "+") {
                vmwriter_.writeArithmetic(Command::ADD);
            }
            else if (op == "-") {
                vmwriter_.writeArithmetic(Command::SUB);
            }
            else if (op == "*") {
                vmwriter_.writeCall("Math.multiply", 2);
            }
            else if (op == "/") {
                vmwriter_.writeCall("Math.divide", 2);
            }
            else if (op == "&") {
                vmwriter_.writeArithmetic(Command::AND);
            }
            else if (op == "|") {
                vmwriter_.writeArithmetic(Command::OR);
            }
            else if (op == "<") {
                vmwriter_.writeArithmetic(Command::LT);
            }
            else if (op == ">") {
                vmwriter_.writeArithmetic(Command::GT);
            }
            else if (op == "=") {
                vmwriter_.writeArithmetic(Command::EQ);
            }
        }

        endTag("expression");
    }

    // OK
    //   integerConstant
    // | stringConstant
    // | keywordConstant
    // | subroutineCall
    // | varName
    // | varName '[' expression ']'
    // | '(' expression ')'
    // | unaryOp term
    void CompilationEngine::compileTerm() {
        startTag("term");

        if (isNextIntergerConstant()) {
            int integer = compileIntegerConstant();
            vmwriter_.writePush(Segment::CONST, integer);
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
                vmwriter_.writeArithmetic(Command::ADD);
                vmwriter_.writePop(Segment::POINTER, 1);
                vmwriter_.writePush(Segment::THAT, 0);
                compileSymbol({ "]" });
            }
        }
        else if (isNext({ "(" })) {
            compileSymbol({ "(" });
            compileExpression();
            compileSymbol({ ")" });
        }
        else if (isNextUnaryOp()) {
            const std::string unary_op = compileUnaryOp();
            compileTerm();
            if (unary_op == "-") {
                vmwriter_.writeArithmetic(Command::NEG);
            }
            else if (unary_op == "~") {
                vmwriter_.writeArithmetic(Command::NOT);
            }
        }

        endTag("term");
    }

    // OK
    // (expression (',' expression)*)?
    int CompilationEngine::compileExpressionList() {
        startTag("expressionList");
        int arg_count = 0;
        if (isNextExpression()) {
            compileExpression();
            ++arg_count;
            while (isNext({ "," })) {
                compileSymbol({ "," });
                compileExpression();
                ++arg_count;
            }
        }

        endTag("expressionList");
        return arg_count;
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

    std::string CompilationEngine::create_label() {
        return "LABEL_" + std::to_string(label_count_++);
    }

    // Helpers
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

}  // namespace nand2tetris
