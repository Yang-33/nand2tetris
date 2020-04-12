#ifndef SOURCE_PROJECT_10_JACKTOKENIZER_H_
#define SOURCE_PROJECT_10_JACKTOKENIZER_H_

#include <queue>
#include <string>
#include <vector>
#include "src_Export.h"


namespace nand2tetris {
    enum class TokenType {
        KEYWORD,
        SYMBOL,
        IDENTIFIER,
        INT_CONST,
        STRING_CONST
    };
    enum class KeywordType {
        CLASS,
        METHOD,
        FUNCTION,
        CONSTRUCTOR,
        INT,
        BOOLEAN,
        CHAR,
        VOID,
        VAR,
        STATIC,
        FIELD,
        LET,
        DO,
        IF,
        ELSE,
        WHILE,
        RETURN,
        TRUE,
        FALSE,
        NULL_,
        THIS
    };
    const std::string kKeyword_Class = "class";
    const std::string kKeyword_Constructor = "constructor";
    const std::string kKeyword_Function = "function";
    const std::string kKeyword_Method = "method";
    const std::string kKeyword_Field = "field";
    const std::string kKeyword_Static = "static";
    const std::string kKeyword_Var = "var";
    const std::string kKeyword_Int = "int";
    const std::string kKeyword_Char = "char";
    const std::string kKeyword_Boolean = "boolean";
    const std::string kKeyword_Void = "void";
    const std::string kKeyword_True = "true";
    const std::string kKeyword_False = "false";
    const std::string kKeyword_Null = "null";
    const std::string kKeyword_This = "this";
    const std::string kKeyword_Let = "let";
    const std::string kKeyword_Do = "do";
    const std::string kKeyword_If = "if";
    const std::string kKeyword_Else = "else";
    const std::string kKeyword_While = "while";
    const std::string kKeyword_Return = "return";

    class src_EXPORT JackTokenizer {
    private:
        std::queue<std::string> token_queue_;
    public:
        explicit JackTokenizer(const std::string&);
        bool hasMoreTokens();
        void advance();
        TokenType tokenType();
        KeywordType keyWord();
        std::string getKeyWord();
        std::string symbol();
        std::string identifier();
        int intVal();
        std::string stringVal();
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_10_JACKTOKENIZER_H_