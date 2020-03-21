#ifndef SOURCE_PROJECT_06_PARSER_H_
#define SOURCE_PROJECT_06_PARSER_H_

#include <string>
#include <queue>
#include "src_Export.h"


namespace nand2tetris {
    enum class COMMAND_TYPE { A_COMMAND, C_COMMAND, L_COMMAND };

    class src_EXPORT Parser {
    private:
        std::queue<std::string> command_queue_;
    public:
        explicit Parser(const std::string);
        bool hasMoreCommands();
        void advance();
        COMMAND_TYPE commandType();
        std::string symbol();
        std::string dest();
        std::string comp();
        std::string jump();
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_06_PARSER_H_