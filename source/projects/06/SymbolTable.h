#ifndef SOURCE_PROJECT_06_SYMBOLTABLE_H_
#define SOURCE_PROJECT_06_SYMBOLTABLE_H_

#include <string>
#include <map>
#include <set>
#include "src_Export.h"


namespace nand2tetris {
    class src_EXPORT SymbolTable {
    private:
        using symbol_table = std::map<std::string, int>;
        symbol_table symbols_;
        std::set<int> used_address_;
    public:
        explicit SymbolTable();
        void addEntry(const std::string& symbol, int address);
        bool contains(const std::string& symbol);
        int getAddress(const std::string& symbol);
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_06_SYMBOLTABLE_H_