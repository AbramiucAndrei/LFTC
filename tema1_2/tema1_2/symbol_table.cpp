#include "symbol_table.h"
#include <fstream>

int SymbolTable::insert(const std::string& symbol) {
    if (indexMap.find(symbol) != indexMap.end())
        return indexMap.at(symbol);

    int pos = (int)table.size();
    table.push_back(symbol);
    indexMap[symbol] = pos;
    return pos;
}

int SymbolTable::lookup(const std::string& symbol) const {
    auto it = indexMap.find(symbol);
    if (it != indexMap.end()) return it->second;
    return -1;
}

void SymbolTable::saveToFile(const std::string& filename) const {
    std::ofstream fout(filename);
    for (size_t i = 0; i < table.size(); ++i)
        fout << i << " : " << table[i] << "\n";
}
