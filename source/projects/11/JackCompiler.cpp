#include <assert.h>
#include <cctype>
#include <fstream>
#include <glob.h>
#include <iostream>
#include <string>

#include "CompilationEngine.h"
#include "JackTokenizer.h"

void part1(nand2tetris::JackTokenizer& tokenizer, const std::string& output_filename) {
    std::ofstream ofs_(output_filename);
    ofs_ << "<tokens>" << std::endl;
    while (tokenizer.hasMoreTokens()) {
        tokenizer.advance();
        nand2tetris::TokenType token_type = tokenizer.tokenType();
        if (token_type == nand2tetris::TokenType::IDENTIFIER) {
            ofs_ << "<identifier>" << tokenizer.identifier() << "</identifier>" << std::endl;
        }
        else if (token_type == nand2tetris::TokenType::INT_CONST) {
            ofs_ << "<integerConstant>" << tokenizer.intVal() << "</integerConstant>" << std::endl;
        }
        else if (token_type == nand2tetris::TokenType::KEYWORD) {
            ofs_ << "<keyword>" << tokenizer.getKeyWord() << "</keyword>" << std::endl;
        }
        else if (token_type == nand2tetris::TokenType::STRING_CONST) {
            ofs_ << "<stringConstant>" << tokenizer.stringVal() << "</stringConstant>" << std::endl;
        }
        else if (token_type == nand2tetris::TokenType::SYMBOL) {
            std::string symbol = tokenizer.symbol();
            if (symbol == "&") {
                symbol = "&amp;";
            }
            else if (symbol == "<") {
                symbol = "&lt;";
            }
            else if (symbol == ">") {
                symbol = "&gt;";
            }
            ofs_ << "<symbol>" << symbol << "</symbol>" << std::endl;
        }
        else {
            assert(false);
        }
    }
    ofs_ << "</tokens>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    std::string path;
    {
        std::string infilepath = argv[1];
        auto last_slash_index = infilepath.find_last_of('/');
        assert(last_slash_index != std::string::npos);
        path = infilepath.substr(0, last_slash_index + 1);
        std::string outfilepath = infilepath.substr(last_slash_index + 1); // /NAME?
        auto dot_index = outfilepath.find_last_of('.');
        if (dot_index == std::string::npos) {
            path = infilepath + "/";
        }
    }

    glob_t glob_result;
    std::string target = path + "*.jack";
    glob(target.c_str(), GLOB_TILDE, NULL, &glob_result);
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        std::string filepath = glob_result.gl_pathv[i];
        std::string xmlpath = filepath.substr(0, filepath.size() - 5) + "-y3.xml";
        if (false) {
            nand2tetris::JackTokenizer tokenizer(filepath);
            part1(tokenizer, xmlpath);
        }
        else {
            nand2tetris::CompilationEngine compilation_engine(filepath, xmlpath);
            compilation_engine.compile();
        }
        std::cerr << "JackAnalyzer created" << filepath << " into " << xmlpath << std::endl;
    }

    return 0;
}
