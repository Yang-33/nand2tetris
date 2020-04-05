#include "Parser.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

namespace nand2tetris {


    std::vector<std::string> split(const std::string& s) {
        std::vector<std::string> res;
        std::stringstream ss(s);
        std::string ret;
        while (ss >> ret) {
            res.push_back(ret);
        }
        return res;
    }
    std::string remove_suffix_space(const std::string& s) {
        for (int i = s.size() - 1; i >= 0; i--) {
            if (!isspace(s[i]))return s.substr(0, i + 1);
        }
        return "";
    }
    Parser::Parser(const std::string& filepath) {
        // Push all commands to |command_queue_| after removing spaces,
        // newlines and comments.
        std::ifstream ifs(filepath);
        if (!ifs) {
            std::cerr << "Can't open " << filepath << "." << std::endl;
            exit(1);
        }

        // Initially there is no current command.
        command_queue_.push({});

        std::string line;
        while (getline(ifs, line)) {
            auto comment_pos = line.find("//");
            line = line.substr(0, comment_pos);
            // Remove space from suffix.
            line = remove_suffix_space(line);
            if (line.size()) {
                command_queue_.push(split(line));
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
    bool is_arithmetic(const std::string& s) {
        return (s.substr(0, kARITHMETIC_ADD.size()) == kARITHMETIC_ADD)
            || (s.substr(0, kARITHMETIC_SUB.size()) == kARITHMETIC_SUB)
            || (s.substr(0, kARITHMETIC_NEG.size()) == kARITHMETIC_NEG)
            || (s.substr(0, kARITHMETIC_EQ.size()) == kARITHMETIC_EQ)
            || (s.substr(0, kARITHMETIC_GT.size()) == kARITHMETIC_GT)
            || (s.substr(0, kARITHMETIC_LT.size()) == kARITHMETIC_LT)
            || (s.substr(0, kARITHMETIC_AND.size()) == kARITHMETIC_AND)
            || (s.substr(0, kARITHMETIC_OR.size()) == kARITHMETIC_OR)
            || (s.substr(0, kARITHMETIC_NOT.size()) == kARITHMETIC_NOT);
    }
    VM_COMMAND_TYPE Parser::commandType() {
        const std::vector<std::string> current_command = command_queue_.front();
        std::string current_command_arg0 = current_command.front();
        if (is_arithmetic(current_command_arg0))
            return VM_COMMAND_TYPE::C_ARITHMETIC;
        if (current_command_arg0.substr(0, kPUSH.size()) == kPUSH)
            return VM_COMMAND_TYPE::C_PUSH;
        if (current_command_arg0.substr(0, kPOP.size()) == kPOP)
            return VM_COMMAND_TYPE::C_POP;
        if (current_command_arg0.substr(0, kLABEL.size()) == kLABEL)
            return VM_COMMAND_TYPE::C_LABEL;
        if (current_command_arg0.substr(0, kGOTO.size()) == kGOTO)
            return VM_COMMAND_TYPE::C_GOTO;
        if (current_command_arg0.substr(0, kIF.size()) == kIF)
            return VM_COMMAND_TYPE::C_IF;
        if (current_command_arg0.substr(0, kFUNCTION.size()) == kFUNCTION)
            return VM_COMMAND_TYPE::C_FUNCTION;
        if (current_command_arg0.substr(0, kRETURN.size()) == kRETURN)
            return VM_COMMAND_TYPE::C_RETURN;
        if (current_command_arg0.substr(0, kCALL.size()) == kCALL)
            return VM_COMMAND_TYPE::C_CALL;
        assert(false);
    }
    std::string Parser::arg1() {
        VM_COMMAND_TYPE current_command_type = commandType();
        assert(current_command_type != VM_COMMAND_TYPE::C_RETURN);
        const std::vector<std::string> current_command = command_queue_.front();
        std::string current_command_arg0 = current_command.front();

        if (current_command_type == VM_COMMAND_TYPE::C_ARITHMETIC) {
            if (current_command_arg0.substr(0, kARITHMETIC_ADD.size()) == kARITHMETIC_ADD)
                return kARITHMETIC_ADD;
            if (current_command_arg0.substr(0, kARITHMETIC_SUB.size()) == kARITHMETIC_SUB)
                return kARITHMETIC_SUB;
            if (current_command_arg0.substr(0, kARITHMETIC_NEG.size()) == kARITHMETIC_NEG)
                return kARITHMETIC_NEG;
            if (current_command_arg0.substr(0, kARITHMETIC_EQ.size()) == kARITHMETIC_EQ)
                return kARITHMETIC_EQ;
            if (current_command_arg0.substr(0, kARITHMETIC_GT.size()) == kARITHMETIC_GT)
                return kARITHMETIC_GT;
            if (current_command_arg0.substr(0, kARITHMETIC_LT.size()) == kARITHMETIC_LT)
                return kARITHMETIC_LT;
            if (current_command_arg0.substr(0, kARITHMETIC_AND.size()) == kARITHMETIC_AND)
                return kARITHMETIC_AND;
            if (current_command_arg0.substr(0, kARITHMETIC_OR.size()) == kARITHMETIC_OR)
                return kARITHMETIC_OR;
            if (current_command_arg0.substr(0, kARITHMETIC_NOT.size()) == kARITHMETIC_NOT)
                return kARITHMETIC_NOT;
            assert(false);
        }
        assert(current_command.size() > 1);
        std::string current_command_arg1 = current_command[1];
        return current_command_arg1;
    }
    int Parser::arg2() {
        VM_COMMAND_TYPE current_command_type = commandType();
        assert(current_command_type != VM_COMMAND_TYPE::C_ARITHMETIC);
        assert(current_command_type != VM_COMMAND_TYPE::C_LABEL);
        assert(current_command_type != VM_COMMAND_TYPE::C_GOTO);
        assert(current_command_type != VM_COMMAND_TYPE::C_IF);
        assert(current_command_type != VM_COMMAND_TYPE::C_RETURN);

        const std::vector<std::string> current_command = command_queue_.front();
        assert(current_command.size() > 2);
        std::string current_command_arg2 = current_command[2];
        return std::stoi(current_command_arg2);
    }

}  // namespace nand2tetris
