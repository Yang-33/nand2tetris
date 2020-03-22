#include "Parser.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

namespace nand2tetris {

    Parser::Parser(const std::string filepath) {
        // Push all commands to |command_queue_| after removing spaces,
        // newlines and comments.
        std::ifstream ifs(filepath);
        if (!ifs) {
            std::cerr << "Can't open " << filepath << "." << std::endl;
            exit(1);
        }

        // Initially there is no current command.
        command_queue_.push("");

        std::string line;
        while (getline(ifs, line)) {
            line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
            if (line.size()) {
                auto comment_pos = line.find("//");
                if (comment_pos == 0)continue;
                line = line.substr(0, comment_pos);
                command_queue_.push(line);
            }
        }

    }
    bool Parser::hasMoreCommands() {
        return command_queue_.size() > 1;
    }
    void Parser::advance() {
        assert(hasMoreCommands());
        command_queue_.pop();
    }
    COMMAND_TYPE Parser::commandType() {
        const std::string current_command = command_queue_.front();
        if (current_command.front() == '@') {
            return COMMAND_TYPE::A_COMMAND;
        }
        else if (current_command.front() == '(') {
            return COMMAND_TYPE::L_COMMAND;
        }
        else {
            return COMMAND_TYPE::C_COMMAND;
        }
    }
    std::string Parser::symbol() {
        COMMAND_TYPE current_command_type = commandType();
        assert(current_command_type != COMMAND_TYPE::C_COMMAND);
        std::string current_command = command_queue_.front();
        if (current_command_type == COMMAND_TYPE::A_COMMAND) {
            // Remove @
            return current_command.substr(1);
        }
        else { // COMMAND_TYPE::L_COMMAND
            // Remove both ends of bracket
            current_command.pop_back();
            return current_command.substr(1);
        }
    }

    // dest=comp;jump
    std::string Parser::dest() {
        {
            COMMAND_TYPE current_command_type = commandType();
            assert(current_command_type == COMMAND_TYPE::C_COMMAND);
        }
        std::string current_command = command_queue_.front();
        auto equal_index = current_command.find_first_of('=');
        if (equal_index == std::string::npos) {
            return "";
        }
        else {
            return current_command.substr(0, equal_index);
        }
    }

    std::string Parser::comp() {
        {
            COMMAND_TYPE current_command_type = commandType();
            assert(current_command_type == COMMAND_TYPE::C_COMMAND);
        }
        std::string current_command = command_queue_.front();
        auto equal_index = current_command.find_first_of('=');
        if (equal_index == std::string::npos) {
            // comp;jump
            auto semicolon_index = current_command.find_first_of(';');
            return current_command.substr(0, semicolon_index);
        }
        else {
            // dest=comp
            // Take command sequence after '='
            return current_command.substr(equal_index + 1);
        }

    }
    std::string Parser::jump() {
        {
            COMMAND_TYPE current_command_type = commandType();
            assert(current_command_type == COMMAND_TYPE::C_COMMAND);
        }
        std::string current_command = command_queue_.front();
        auto semicolon_index = current_command.find_first_of(';');
        if (semicolon_index == std::string::npos) {
            return "";
        }
        else {
            return current_command.substr(semicolon_index + 1);
        }
    }
}  // namespace nand2tetris
