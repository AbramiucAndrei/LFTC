#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

enum class TokenType {
    ID, CONST_INT, CONST_REAL, CONST_STRING,
    KW_INT, KW_FLOAT, KW_STRING, KW_IF, KW_ELSE, KW_WHILE, KW_CIN, KW_COUT, KW_RETURN,
    OP_ASSIGN, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_MOD,
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_SHIFT_LEFT, OP_SHIFT_RIGHT,
    SEP_SEMICOLON, SEP_COMMA, SEP_LPAREN, SEP_RPAREN, SEP_LBRACE, SEP_RBRACE,
    UNKNOWN, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int posInTS;

    Token(TokenType t = TokenType::UNKNOWN, std::string l = "", int ln = 0, int ts = -1)
        : type(t), lexeme(std::move(l)), line(ln), posInTS(ts) {
    }
};

std::string tokenTypeToString(TokenType t);

#endif
