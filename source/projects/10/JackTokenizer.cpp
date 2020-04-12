#include "JackTokenizer.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

namespace nand2tetris {

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (getline(ss, item, delim)) {
            if (!item.empty()) {
                elems.push_back(item);
            }
        }
        return elems;
    }

    JackTokenizer::JackTokenizer(const std::string& filepath) {
        // Push all commands to |token_queue_| after removing spaces,
        // newlines and comments.
        std::ifstream ifs(filepath);
        if (!ifs) {
            std::cerr << "Can't open " << filepath << "." << std::endl;
            exit(1);
        }
        auto trim = [](std::string s) {
            int start;
            for (start = 0; start < s.size() && std::isspace(s[start]); ++start);
            int end;
            for (end = s.size() - 1; end >= 0 && std::isspace(s[end]); --end);
            return s.substr(start, end - start + 1);
        };
        std::vector<std::string>lines; {
            // to oneline
            std::string ss; {
                std::string s;
                while (getline(ifs, s)) {
                    ss += s + "\n";
                }
            }

            // /**/ コメントの削除
            while (ss.find("/*") != std::string::npos) {
                auto index = ss.find("/*");
                auto index2 = ss.find("*/");
                ss = ss.substr(0, index) + ss.substr(index2 + 2);
            }
            lines = split(ss, '\n');

            for (auto& line : lines) {
                if (trim(line).size() && trim(line).find("//") != 0) {
                    ;
                }
                else {
                    line.clear();
                }
            }

            // // コメントの削除
            for (auto& line : lines) {
                if (line.find("//") != std::string::npos) {
                    auto idx = line.find("//");
                    line = line.substr(0, idx);
                    line = trim(line);
                }
            }
            std::vector<std::string> valid_lines;
            for (auto s : lines) {
                if (s.size()) {
                    valid_lines.push_back(s);
                }
            }
            valid_lines.swap(lines);
        }
        token_queue_.push({});
        auto PUSH = [&](std::string s) {
            //std::cerr<<__func__<<", "<<s<<std::endl;
            s = trim(s);
            if (s.size()) {
                token_queue_.push(s);
            }
        };
        // 分離
        // matchするならば、前方で一致するものを探す
        auto match = [](const std::string& s) {
            size_t cnt = 0;
            for (auto c : s) {
                for (auto symbol : { '{','}','(',')','[',']','.',',',';','+','-',
                    '*','/','&','|','<','>','=','~' }) {
                    if (c == symbol) return cnt;
                }
                ++cnt;
            }
            return std::string::npos;
        };
        auto parserUnit = [&](std::string& unit) {
            while (unit.size()) {
                auto i = match(unit);
                if (match(unit) != std::string::npos) {
                    const auto index = match(unit);
                    if (index != std::string::npos) {
                        PUSH(unit.substr(0, index));
                    }
                    PUSH(unit.substr(index, 1));
                    unit = unit.substr(index + 1);
                }
                else {
                    PUSH(unit);
                    unit.clear();
                }
            }
        };
        for (auto line : lines) {
            while (line.size()) {
                const auto doubleQuoteIndex = line.find('"');
                const auto spaceIndex = line.find(' ');
                if (line.find('"') == 0) {
                    const auto index = line.find('"', 1);
                    PUSH(line.substr(0, index + 1));
                    line = line.substr(index + 1);
                    line = trim(line);
                }
                else if (doubleQuoteIndex != std::string::npos && spaceIndex != std::string::npos && doubleQuoteIndex < spaceIndex) {
                    auto unit = line.substr(0, doubleQuoteIndex);
                    parserUnit(unit);
                    line = line.substr(doubleQuoteIndex);
                    line = trim(line);
                }
                else if (spaceIndex != std::string::npos) {
                    auto unit = line.substr(0, spaceIndex);
                    parserUnit(unit);
                    line = line.substr(spaceIndex + 1);
                    line = trim(line);
                }
                else {
                    parserUnit(line);
                    line.clear();
                }
            }
        }
    }

    bool JackTokenizer::hasMoreTokens() {
        return token_queue_.size() > 1;
    }

    void JackTokenizer::advance() {
        assert(hasMoreTokens());
        token_queue_.pop();
    }

    TokenType JackTokenizer::tokenType() {

        // KeyWord
        const std::string token = token_queue_.front();
        {
            for (auto keyword : { kKeyword_Class, kKeyword_Constructor,
                kKeyword_Function, kKeyword_Method, kKeyword_Field,
                kKeyword_Static, kKeyword_Var, kKeyword_Int,
                kKeyword_Char, kKeyword_Boolean, kKeyword_Void,
                kKeyword_True, kKeyword_False, kKeyword_Null,
                kKeyword_This, kKeyword_Let, kKeyword_Do, kKeyword_If,
                kKeyword_Else, kKeyword_While, kKeyword_Return }) {
                if (token == keyword) {
                    return TokenType::KEYWORD;
                }
            }
        }
        // Symbol
        {
            for (auto symbol : { '{','}','(',')','[',']','.',',',';','+','-',
                '*','/','&','|','<','>','=','~' }) {
                if (token.front() == symbol) {
                    return TokenType::SYMBOL;
                }
            }
        }
        // interger
        {
            bool is_int = true; {
                for (char c : token) {
                    is_int &= std::isdigit(c);
                }
            }

            if (is_int) {
                return TokenType::INT_CONST;
            }
        }
        // string
        {
            if (token.front() == '"' && token.back() == '"') {
                return TokenType::STRING_CONST;
            }
        }
        // identifier
        return TokenType::IDENTIFIER;
    }
    KeywordType JackTokenizer::keyWord() {
        assert(tokenType() == nand2tetris::TokenType::KEYWORD);
        const std::string keyword = token_queue_.front();
        if (keyword == kKeyword_Boolean) {
            return KeywordType::BOOLEAN;
        }
        else if (keyword == kKeyword_Char) {
            return KeywordType::CHAR;
        }
        else if (keyword == kKeyword_Class) {
            return KeywordType::CLASS;
        }
        else if (keyword == kKeyword_Constructor) {
            return KeywordType::CONSTRUCTOR;
        }
        else if (keyword == kKeyword_Do) {
            return KeywordType::DO;
        }
        else if (keyword == kKeyword_Else) {
            return KeywordType::ELSE;
        }
        else if (keyword == kKeyword_False) {
            return KeywordType::FALSE;
        }
        else if (keyword == kKeyword_Field) {
            return KeywordType::FIELD;
        }
        else if (keyword == kKeyword_Function) {
            return KeywordType::FUNCTION;
        }
        else if (keyword == kKeyword_If) {
            return KeywordType::IF;
        }
        else if (keyword == kKeyword_Int) {
            return KeywordType::INT;
        }
        else if (keyword == kKeyword_Let) {
            return KeywordType::LET;
        }
        else if (keyword == kKeyword_Method) {
            return KeywordType::METHOD;
        }
        else if (keyword == kKeyword_Null) {
            return KeywordType::NULL_;
        }
        else if (keyword == kKeyword_Return) {
            return KeywordType::RETURN;
        }
        else if (keyword == kKeyword_Static) {
            return KeywordType::STATIC;
        }
        else if (keyword == kKeyword_This) {
            return KeywordType::THIS;
        }
        else if (keyword == kKeyword_True) {
            return KeywordType::TRUE;
        }
        else if (keyword == kKeyword_Var) {
            return KeywordType::VAR;
        }
        else if (keyword == kKeyword_Void) {
            return KeywordType::VOID;
        }
        else if (keyword == kKeyword_While) {
            return KeywordType::WHILE;
        }
        else {
            assert(false);
        }
    }
    std::string JackTokenizer::getKeyWord() {
        assert(tokenType() == nand2tetris::TokenType::KEYWORD);
        return token_queue_.front();
    }

    std::string JackTokenizer::symbol() {
        assert(tokenType() == nand2tetris::TokenType::SYMBOL);
        return token_queue_.front();
    }
    std::string JackTokenizer::identifier() {
        assert(tokenType() == nand2tetris::TokenType::IDENTIFIER);
        return token_queue_.front();
    }
    int JackTokenizer::intVal() {
        assert(tokenType() == nand2tetris::TokenType::INT_CONST);
        return std::stoi(token_queue_.front());
    }
    std::string JackTokenizer::stringVal() {
        assert(tokenType() == nand2tetris::TokenType::STRING_CONST);
        std::string token = token_queue_.front();
        return std::string(token.begin() + 1, token.end() - 1);
    }
}  // namespace nand2tetris
