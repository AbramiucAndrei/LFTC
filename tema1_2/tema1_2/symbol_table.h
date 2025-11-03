#pragma once
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>

class SymbolTable {
private:
    std::vector<std::string> table;
    std::unordered_map<std::string, int> indexMap;

public:
    int insert(const std::string& symbol);
    int lookup(const std::string& symbol) const;
    void saveToFile(const std::string& filename) const;
};

#endif
