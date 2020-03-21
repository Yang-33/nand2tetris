#include "Code.h"

#include <assert.h>
#include <bitset>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace nand2tetris {
    const std::vector<std::pair<const std::string, int>> kCOMP_LIST = {
        // a=0
        { "0", 0b0101010 },
        { "1", 0b0111111 },
        { "-1", 0b0111010 },
        { "D", 0b0001100 },
        { "A", 0b0110000 },

        { "!D", 0b0001101 },
        { "!A", 0b0110001 },
        { "-D", 0b0001111 },
        { "-A", 0b0110011 },
        { "D+1", 0b0011111 },

        { "A+1", 0b0110111 },
        { "D-1", 0b0001110 },
        { "A-1", 0b0110010 },
        { "D+A", 0b0000010 },
        { "D-A", 0b0010011 },

        { "A-D", 0b0000111 },
        { "D&A", 0b0000000 },
        { "D|A", 0b0010101 },

        // a=1
        { "M", 0b1110000 },
        { "!M", 0b1110001 },
        { "-M", 0b1110011 },
        { "M+1", 0b1110111 },
        { "M-1", 0b1110010 },

        { "D+M", 0b1000010 },
        { "D-M", 0b1010011 },
        { "M-D", 0b1000111 },
        { "D&M", 0b1000000 },
        { "D|M", 0b1010101 }
    };

    const std::vector<std::pair<const std::string, int>> kDEST_LIST = {
        { "", 0b000 },
        { "M", 0b001 },
        { "D", 0b010 },
        { "MD", 0b011 },
        { "A", 0b100 },
        { "AM", 0b101 },
        { "AD", 0b110 },
        { "AMD", 0b111 }
    };

    const std::vector<std::pair<const std::string, int>> kJUMP_LIST = {
        { "", 0b000 },
        { "JGT", 0b001 },
        { "JEQ", 0b010 },
        { "JGE", 0b011 },
        { "JLT", 0b100 },
        { "JNE", 0b101 },
        { "JLE", 0b110 },
        { "JMP", 0b111 }
    };

    Code::Code() :
        comp_(kCOMP_LIST.begin(), kCOMP_LIST.end()),
        dest_(kDEST_LIST.begin(), kDEST_LIST.end()),
        jump_(kJUMP_LIST.begin(), kJUMP_LIST.end()) {}

    int Code::comp(const std::string& s) {
        auto comp_bit = comp_.find(s);
        if (comp_bit == comp_.end()) {
            std::cerr << __func__ << "'s argument is " << s << std::endl;
            assert(false);
        }
        return comp_bit->second;
    }

    int Code::dest(const std::string& s) {
        auto dest_bit = dest_.find(s);
        if (dest_bit == dest_.end()) {
            std::cerr << __func__ << "'s argument is " << s << std::endl;
            assert(false);
        }
        return dest_bit->second;
    }

    int Code::jump(const std::string& s) {
        auto jump_bit = jump_.find(s);
        if (jump_bit == jump_.end()) {
            std::cerr << __func__ << "'s argument is " << s << std::endl;
            assert(false);
        }
        return jump_bit->second;
    }
    std::string Code::ACode(int address) {
        std::bitset<16> ans(address);
        return ans.to_string();
    }
    std::string Code::CCode(const std::string& comps, const std::string& dests, const std::string& jumps) {
        std::stringstream ss;
        ss << (0b111 << 13)
            + (comp(comps) << 6)
            + (dest(dests) << 3)
            + (jump(jumps));
        std::bitset<16> ans(stoi(ss.str()));
        return ans.to_string();
    }
}  // namespace nand2tetris
