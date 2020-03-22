#include <assert.h>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

#include "Code.h"
#include "Parser.h"
#include "SymbolTable.h"

namespace nand2tetris {
    class Assembler {
    public:
        explicit Assembler(const std::string& infilepath,
            const std::string& outfilepath) :
            code_(), parser_(infilepath), symbol_table_(), symbol_address_(16),
            infilepath_(infilepath), ofs_(outfilepath) {
        }

        void assemble() {
            first_path();
            second_path();
        }
    private:
        void first_path() {
            int current_address = 0;
            while (parser_.hasMoreCommands()) {
                parser_.advance();
                COMMAND_TYPE current_command_type = parser_.commandType();
                if (current_command_type == COMMAND_TYPE::A_COMMAND
                    || current_command_type == COMMAND_TYPE::C_COMMAND) {
                    ++current_address;
                }
                else {
                    symbol_table_.addEntry(parser_.symbol(), current_address);
                }
            }
        }
        void second_path() {
            parser_ = Parser(infilepath_);
            while (parser_.hasMoreCommands()) {
                parser_.advance();
                COMMAND_TYPE current_command_type = parser_.commandType();
                if (current_command_type == COMMAND_TYPE::A_COMMAND) {
                    ofs_ << code_.ACode(get_address(parser_.symbol()));
                    ofs_ << "\n";
                }
                else if (current_command_type == COMMAND_TYPE::C_COMMAND) {
                    ofs_ << code_.CCode(parser_.comp(), parser_.dest(), parser_.jump());
                    ofs_ << "\n";
                }
                else if (current_command_type == COMMAND_TYPE::L_COMMAND) {
                    continue;
                }
            }
        }
        int get_address(const std::string& symbol) {
            if (isdigit(symbol))return stoi(symbol);
            if (!symbol_table_.contains(symbol)) {// don't contain its Acode symbol.
                symbol_table_.addEntry(symbol, symbol_address_++);
            }
            return symbol_table_.getAddress(symbol);
        }
        bool isdigit(const std::string& symbol) {
            for (auto c : symbol) {
                if (!std::isdigit(c))return false;
            }
            return true;
        }
        Code code_;
        Parser parser_;
        SymbolTable symbol_table_;
        int symbol_address_;
        std::string infilepath_;
        std::ofstream ofs_;
    };
}  // namespace nand2tetris


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << __func__ << " needs arguments 2." << std::endl;
        return 1;
    }
    // TODO(yutakasai): impl 6.3.5
    std::string infilepath = argv[1];
    std::string outfilepath; {
        auto dot_index = infilepath.find_last_of('.');
        outfilepath = infilepath.substr(0, dot_index) + "-y3.hack";
    }
    nand2tetris::Assembler assembler(infilepath, outfilepath);
    assembler.assemble();
    std::cout << "Assembler assembled " << infilepath << " into " << outfilepath << std::endl;

    return 0;
}