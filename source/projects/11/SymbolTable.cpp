#include "SymbolTable.h"

#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

namespace nand2tetris {

    SymbolTable::SymbolTable()
        :static_var_count_(0), field_var_count_(0),
        arg_var_count_(0), local_var_count_(0) {}

    void SymbolTable::startSubroutine() {
        subroutine_scope_.clear();
        arg_var_count_ = 0;
        local_var_count_ = 0;
    }

    bool SymbolTable::hasKey(std::map<std::string, SymbolTuple>& scope, const std::string& s) {
        return scope.find(s) != scope.end();
    }

    void SymbolTable::define(
        const std::string& name, const std::string& type, SymbolType kind) {
        if (kind == SymbolType::STATIC) {
            assert(!hasKey(class_scope_, name));
            class_scope_[name] = SymbolTuple(type, kind, static_var_count_);
            ++static_var_count_;
        }
        else if (kind == SymbolType::FIELD) {
            assert(!hasKey(class_scope_, name));
            class_scope_[name] = SymbolTuple(type, kind, field_var_count_);
            ++field_var_count_;
        }
        else if (kind == SymbolType::ARG) {
            assert(!hasKey(subroutine_scope_, name));
            subroutine_scope_[name] = SymbolTuple(type, kind, arg_var_count_);
            ++arg_var_count_;
        }
        else if (kind == SymbolType::VAR) {
            assert(!hasKey(subroutine_scope_, name));
            subroutine_scope_[name] = SymbolTuple(type, kind, local_var_count_);
            ++local_var_count_;
        }
        else {
            std::cerr << __func__ << "," << name << "," << type << "," << (int)kind << std::endl;
            assert(false);
        }
    }

    int SymbolTable::varCount(SymbolType kind) {
        if (kind == SymbolType::STATIC) {
            return static_var_count_;
        }
        else if (kind == SymbolType::FIELD) {
            return field_var_count_;
        }
        else if (kind == SymbolType::ARG) {
            return arg_var_count_;
        }
        else if (kind == SymbolType::VAR) {
            return local_var_count_;
        }
        else {
            assert(false);
            return 0;
        }
    }

    SymbolType SymbolTable::kindOf(const std::string& name) {
        if (hasKey(subroutine_scope_, name)) {
            return subroutine_scope_[name].kind_;
        }
        else if (hasKey(class_scope_, name)) {
            return class_scope_[name].kind_;
        }
        else {
            return SymbolType::NONE;
        }
    }

    std::string SymbolTable::typeOf(const std::string& name) {
        if (hasKey(subroutine_scope_, name)) {
            return subroutine_scope_[name].type_;
        }
        else if (hasKey(class_scope_, name)) {
            return class_scope_[name].type_;
        }
        else {
            std::cerr << __func__ << "," << name << std::endl;
            assert(false);
            return "";
        }
    }

    int SymbolTable::indexOf(const std::string& name) {
        if (hasKey(subroutine_scope_, name)) {
            return subroutine_scope_[name].index_;
        }
        else if (hasKey(class_scope_, name)) {
            return class_scope_[name].index_;
        }
        else {
            std::cerr << __func__ << "," << name << std::endl;
            assert(false);
            return -1;
        }
    }


}  // namespace nand2tetris
