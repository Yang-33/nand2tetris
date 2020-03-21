#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>

#include "Code.h"
#include "Parser.h"
#include "SymbolTable.h"

namespace nand2tetris {
    class Assembler {
    public:
        explicit Assembler(const std::string& infilepath,
            const std::string& outfilepath) :
            code_(), parser_(infilepath), symbol_table_(), ofs_(outfilepath) {
        }

        void assemble() {
            first_path();
            second_path();
        }
        void first_path() {

        }
        void second_path() {

        }
    private:
        Code code_;
        Parser parser_;
        SymbolTable symbol_table_;
        std::ofstream ofs_;
    };
}  // namespace nand2tetris


int main(int argc, char* argv[]) {
    std::cout << "Assembler code called!" << std::endl;
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

    return 0;
}