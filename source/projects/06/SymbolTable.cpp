#include "SymbolTable.h"

#include <assert.h>
#include <vector>

namespace nand2tetris {

    const std::vector<std::pair<const std::string, int>> kSYMBOL_LIST = {
        { "SP", 0x0000 },
        { "LCL", 0x0001 },
        { "ARG", 0x0010 },
        { "THIS", 0x0011 },
        { "THAT", 0x0100 },
        { "R0", 0x0000 },
        { "R1", 0x0001 },
        { "R2", 0x0010 },
        { "R3", 0x0011 },
        { "R4", 0x0100 },
        { "R5", 0x0101 },
        { "R6", 0x0110 },
        { "R7", 0x0111 },
        { "R8", 0x1000 },
        { "R9", 0x1001 },
        { "R10", 0x1010 },
        { "R11", 0x1011 },
        { "R12", 0x1100 },
        { "R13", 0x1101 },
        { "R14", 0x1110 },
        { "R15", 0x1111 },
        { "SCREEN", 0x4000 },
        { "KBD", 0x6000 }
    };

    SymbolTable::SymbolTable() :
        symbols_(kSYMBOL_LIST.begin(), kSYMBOL_LIST.end()) {}

    void SymbolTable::addEntry(const std::string& symbol, int address) {
        assert(!contains(symbol));
        assert(!used_address_.count(address));
        symbols_[symbol] = address;
        used_address_.insert(address);
    }
    bool SymbolTable::contains(const std::string& symbol) {
        return symbols_.find(symbol) != symbols_.end();
    }
    int SymbolTable::getAddress(const std::string& symbol) {
        assert(contains(symbol));
        return symbols_[symbol];
    }
}  // namespace nand2tetris
