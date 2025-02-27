#ifndef SOURCE_PROJECT_11_SYMBOLTABLE_H_
#define SOURCE_PROJECT_11_SYMBOLTABLE_H_

#include <deque>
#include <map>
#include <string>
#include "src_Export.h"


namespace nand2tetris {

    enum class SymbolType {
        STATIC,
        FIELD,
        ARG,
        VAR,
        NONE
    };
    class src_EXPORT SymbolTable {
    public:
        explicit SymbolTable();
        void startSubroutine();

        // 割当（名前、型、シンボルのタイプ）
        void define(const std::string& name, const std::string& type, SymbolType kind);

        // 引数で与えられた属性（シンボルのタイプ）について、それが現在のスコープで与えられている数
        int varCount(SymbolType kind);

        // 引数で与えら得た名前の識別子を現在のスコープで探し、その属性を返す。
        // その識別子が現在のスコープで見つからなければNONEを返す
        SymbolType kindOf(const std::string& name);

        // 引数で与えられた名前の識別子を現在のスコープで探し、その型を返す
        std::string typeOf(const std::string& name);

        // 引数で与えられた名前の識別子を現在のスコープで探し、そのインデックスを返す
        int indexOf(const std::string& name);
    private:
        struct SymbolTuple {
            SymbolTuple() {}
            SymbolTuple(const std::string& type, SymbolType kind, int index)
                :type_(type), kind_(kind), index_(index) {}
            std::string type_;
            SymbolType kind_;
            int index_;
        };
        bool hasKey(std::map<std::string, SymbolTuple>& scope, const std::string& name);
        std::map<std::string, SymbolTuple> class_scope_;
        std::map<std::string, SymbolTuple> subroutine_scope_;
        int static_var_count_;
        int field_var_count_;
        int arg_var_count_;
        int local_var_count_;
    };
}  // namespace nand2tetris

#endif // SOURCE_PROJECT_11_SYMBOLTABLE_H_