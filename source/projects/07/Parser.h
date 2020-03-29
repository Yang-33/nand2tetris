#ifndef SOURCE_PROJECT_07_PARSER_H_
#define SOURCE_PROJECT_07_PARSER_H_

#include <queue>
#include <string>
#include <vector>
#include "src_Export.h"


namespace nand2tetris {

    enum class VM_COMMAND_TYPE {
        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL
    };

    const std::string kARITHMETIC_ADD = "add";
    const std::string kARITHMETIC_SUB = "sub";
    const std::string kARITHMETIC_NEG = "neg";
    const std::string kARITHMETIC_EQ = "eq";
    const std::string kARITHMETIC_GT = "gt";
    const std::string kARITHMETIC_LT = "lt";
    const std::string kARITHMETIC_AND = "and";
    const std::string kARITHMETIC_OR = "or";
    const std::string kARITHMETIC_NOT = "not";

    const std::string kPUSH = "push";

    const std::string kPOP = "pop";

    const std::string kLABEL = "label";

    const std::string kGOTO = "goto";

    const std::string kIF = "if-goto";

    const std::string kFUNCTION = "function";

    const std::string kRETURN = "return";

    const std::string kCALL = "call";

    class src_EXPORT Parser {
    private:
        std::queue<std::vector<std::string>> command_queue_;
    public:
        explicit Parser(const std::string&);
        bool hasMoreCommands();
        void advance();
        VM_COMMAND_TYPE commandType();
        std::string arg1();
        int arg2();
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_07_PARSER_H_