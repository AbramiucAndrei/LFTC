#include "token.h"

std::string tokenTypeToString(TokenType t) {
    static std::unordered_map<TokenType, std::string> names = {
        {TokenType::ID, "ID"}, {TokenType::CONST_INT, "CONST_INT"},
        {TokenType::CONST_REAL, "CONST_REAL"}, {TokenType::CONST_STRING, "CONST_STRING"},
        {TokenType::KW_INT, "int"}, {TokenType::KW_FLOAT, "float"},
        {TokenType::KW_STRING, "string"}, {TokenType::KW_IF, "if"},
        {TokenType::KW_ELSE, "else"}, {TokenType::KW_WHILE, "while"},
        {TokenType::KW_CIN, "cin"}, {TokenType::KW_COUT, "cout"},
        {TokenType::KW_RETURN, "return"}, {TokenType::OP_ASSIGN, "="},
        {TokenType::OP_PLUS, "+"}, {TokenType::OP_MINUS, "-"},
        {TokenType::OP_MUL, "*"}, {TokenType::OP_DIV, "/"},
        {TokenType::OP_MOD, "%"}, {TokenType::OP_EQ, "=="},
        {TokenType::OP_NEQ, "!="}, {TokenType::OP_LT, "<"},
        {TokenType::OP_GT, ">"}, {TokenType::OP_LE, "<="},
        {TokenType::OP_GE, ">="}, {TokenType::OP_SHIFT_LEFT, "<<"},
        {TokenType::OP_SHIFT_RIGHT, ">>"}, {TokenType::SEP_SEMICOLON, ";"},
        {TokenType::SEP_COMMA, ","}, {TokenType::SEP_LPAREN, "("},
        {TokenType::SEP_RPAREN, ")"}, {TokenType::SEP_LBRACE, "{"},
        {TokenType::SEP_RBRACE, "}"}, {TokenType::UNKNOWN, "UNKNOWN"},
        {TokenType::END_OF_FILE, "EOF"}
    };
    return names[t];
}
