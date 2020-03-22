#ifndef SOURCE_PROJECT_06_CODE_H_
#define SOURCE_PROJECT_06_CODE_H_

#include <string>
#include <map>
#include "src_Export.h"


namespace nand2tetris {
    class src_EXPORT Code {
    private:
        using mnemonic_to_bit = std::map<std::string, int>;
        mnemonic_to_bit comp_;
        mnemonic_to_bit dest_;
        mnemonic_to_bit jump_;

    public:
        explicit Code();
        int comp(const std::string&);
        int dest(const std::string&);
        int jump(const std::string&);
        std::string ACode(int address);
        std::string CCode(const std::string&, const std::string&, const std::string&);
    };
}  // namespace nand2tetris

#endif  // SOURCE_PROJECT_06_CODE_H_