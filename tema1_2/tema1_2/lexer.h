#pragma once
#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "symbol_table.h"
#include <vector>
#include <string>

class Lexer {
private:
    std::string source;
    int line = 1;
    SymbolTable& ts;

public:
    explicit Lexer(SymbolTable& table);
    void loadSource(const std::string& filename);
    std::vector<Token> tokenize();
    void saveFIP(const std::vector<Token>& tokens, const std::string& filename);
    void saveErrors(const std::vector<std::string>& errors, const std::string& filename);
};

#endif
