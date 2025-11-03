#include "lexer.h"
#include "symbol_table.h"
#include <iostream>

int main() {
    SymbolTable ts;
    Lexer lexer(ts);

    lexer.loadSource("err2.txt");

    std::vector<Token> tokens = lexer.tokenize();

    lexer.saveFIP(tokens, "fip.txt");
    ts.saveToFile("ts.txt");

    std::cout << "Analiza finalizata.\n";
    return 0;
}
