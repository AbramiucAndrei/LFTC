#include "lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <cctype>

using namespace std;

// ------------------------------------------------------------
// CONSTRUCTOR & INITIALIZARE
// ------------------------------------------------------------

Lexer::Lexer(SymbolTable& table) : ts(table) {}

void Lexer::loadSource(const std::string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Eroare: nu s-a putut deschide fisierul sursa " << filename << "\n";
        exit(1);
    }
    source.assign((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();
}

// ------------------------------------------------------------
// FUNCTII UTILE
// ------------------------------------------------------------

static bool isLetter(char c) {
    return isalpha(static_cast<unsigned char>(c));
}

static bool isDigit(char c) {
    return isdigit(static_cast<unsigned char>(c));
}

static bool isLetterOrDigit(char c) {
    return isalnum(static_cast<unsigned char>(c));
}

static bool isKeyword(const string& s) {
    static const vector<string> keywords = {
        "int", "float", "string", "if", "else", "while", "cin", "cout", "return"
    };
    for (const auto& kw : keywords)
        if (s == kw) return true;
    return false;
}

// ------------------------------------------------------------
// TOKENIZARE
// ------------------------------------------------------------

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    vector<string> errors;
    size_t i = 0;
    int n = source.size();

    while (i < n) {
        char c = source[i];

        // ----------------------------------------------------
        // Ignora spatiile, taburile si newline-urile
        // ----------------------------------------------------
        if (isspace(c)) {
            if (c == '\n') line++;
            i++;
            continue;
        }

        // ----------------------------------------------------
        // COMENTARII: // ... pana la sfarsitul liniei
        // ----------------------------------------------------
        if (c == '/' && i + 1 < n && source[i + 1] == '/') {
            while (i < n && source[i] != '\n') i++;
            continue;
        }

        // ----------------------------------------------------
        // STRINGURI între " "
        // ----------------------------------------------------
        if (c == '"') {
            size_t start = i++;
            string val;
            while (i < n && source[i] != '"') {
                if (source[i] == '\n') {
                    errors.push_back("Eroare la linia " + to_string(line) +
                        ": string neterminat");
                    break;
                }
                val += source[i++];
            }
            if (i < n && source[i] == '"') i++; // inchide ghilimelele
            int posTS = ts.insert(val);
            tokens.emplace_back(TokenType::CONST_STRING, val, line, posTS);
            continue;
        }

        // ----------------------------------------------------
        // IDENTIFICATORI sau CUVINTE CHEIE
        // ----------------------------------------------------
        if (isLetter(c)) {
            string id;
            id += c;
            i++;
            while (i < n && isLetterOrDigit(source[i])) {
                id += source[i++];
            }

            if (!islower(id[0])) {
                errors.push_back("Eroare la linia " + to_string(line) +
                    ": identificatorul '" + id +
                    "' incepe cu majuscula (invalid in MLP)");
            }

            if (isKeyword(id)) {
                TokenType t;
                if (id == "int") t = TokenType::KW_INT;
                else if (id == "float") t = TokenType::KW_FLOAT;
                else if (id == "string") t = TokenType::KW_STRING;
                else if (id == "if") t = TokenType::KW_IF;
                else if (id == "else") t = TokenType::KW_ELSE;
                else if (id == "while") t = TokenType::KW_WHILE;
                else if (id == "cin") t = TokenType::KW_CIN;
                else if (id == "cout") t = TokenType::KW_COUT;
                else t = TokenType::KW_RETURN;

                tokens.emplace_back(t, id, line, -1);
            }
            else {
                int posTS = ts.insert(id);
                tokens.emplace_back(TokenType::ID, id, line, posTS);
            }
            continue;
        }

        // ----------------------------------------------------
        // NUMERE INT / FLOAT (semn optional)
        // ----------------------------------------------------
        if (c == '+' || c == '-' || isDigit(c)) {
            size_t start = i;
            bool hasSign = (c == '+' || c == '-');
            if (hasSign) i++;
            bool hasDigits = false, hasDot = false;

            while (i < n && isDigit(source[i])) {
                hasDigits = true;
                i++;
            }
            if (i < n && source[i] == '.') {
                hasDot = true;
                i++;
                while (i < n && isDigit(source[i])) {
                    hasDigits = true;
                    i++;
                }
            }

            if (!hasDigits) {
                errors.push_back("Eroare la linia " + to_string(line) +
                    ": semn izolat sau numar invalid");
                i++;
                continue;
            }

            string val = source.substr(start, i - start);
            int posTS = ts.insert(val);

            if (hasDot)
                tokens.emplace_back(TokenType::CONST_REAL, val, line, posTS);
            else
                tokens.emplace_back(TokenType::CONST_INT, val, line, posTS);
            continue;
        }

        // ----------------------------------------------------
        // OPERATORI SI SEPARATORI
        // ----------------------------------------------------
        string twoChars;
        if (i + 1 < n) twoChars = string() + c + source[i + 1];

        if (twoChars == "==") { tokens.emplace_back(TokenType::OP_EQ, twoChars, line, -1); i += 2; continue; }
        if (twoChars == "!=") { tokens.emplace_back(TokenType::OP_NEQ, twoChars, line, -1); i += 2; continue; }
        if (twoChars == "<=") { tokens.emplace_back(TokenType::OP_LE, twoChars, line, -1); i += 2; continue; }
        if (twoChars == ">=") { tokens.emplace_back(TokenType::OP_GE, twoChars, line, -1); i += 2; continue; }
        if (twoChars == ">>") { tokens.emplace_back(TokenType::OP_SHIFT_RIGHT, twoChars, line, -1); i += 2; continue; }
        if (twoChars == "<<") { tokens.emplace_back(TokenType::OP_SHIFT_LEFT, twoChars, line, -1); i += 2; continue; }

        switch (c) {
        case '=': tokens.emplace_back(TokenType::OP_ASSIGN, "=", line, -1); break;
        case '+': tokens.emplace_back(TokenType::OP_PLUS, "+", line, -1); break;
        case '-': tokens.emplace_back(TokenType::OP_MINUS, "-", line, -1); break;
        case '*': tokens.emplace_back(TokenType::OP_MUL, "*", line, -1); break;
        case '/': tokens.emplace_back(TokenType::OP_DIV, "/", line, -1); break;
        case '%': tokens.emplace_back(TokenType::OP_MOD, "%", line, -1); break;
        case '<': tokens.emplace_back(TokenType::OP_LT, "<", line, -1); break;
        case '>': tokens.emplace_back(TokenType::OP_GT, ">", line, -1); break;
        case ';': tokens.emplace_back(TokenType::SEP_SEMICOLON, ";", line, -1); break;
        case ',': tokens.emplace_back(TokenType::SEP_COMMA, ",", line, -1); break;
        case '(': tokens.emplace_back(TokenType::SEP_LPAREN, "(", line, -1); break;
        case ')': tokens.emplace_back(TokenType::SEP_RPAREN, ")", line, -1); break;
        case '{': tokens.emplace_back(TokenType::SEP_LBRACE, "{", line, -1); break;
        case '}': tokens.emplace_back(TokenType::SEP_RBRACE, "}", line, -1); break;
        default:
            errors.push_back("Eroare la linia " + to_string(line) +
                ": simbol necunoscut '" + string(1, c) + "'");
            break;
        }
        i++;
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "EOF", line, -1);
    saveErrors(errors, "errors.txt");
    return tokens;
}

// ------------------------------------------------------------
// SALVARE FIP + ERORI
// ------------------------------------------------------------

void Lexer::saveFIP(const vector<Token>& tokens, const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Eroare: nu s-a putut crea fisierul " << filename << "\n";
        return;
    }

    fout << "COD_TOKEN | POZ_TS | LEXEM | LINIE\n";
    fout << "-----------------------------------\n";

    for (const auto& t : tokens) {
        fout << (int)t.type << " | ";
        fout << (t.posInTS == -1 ? "-" : to_string(t.posInTS)) << " | ";
        fout << t.lexeme << " | ";
        fout << t.line << "\n";
    }
    fout.close();
}

void Lexer::saveErrors(const vector<string>& errors, const string& filename) {
    ofstream fout(filename);
    for (const auto& e : errors)
        fout << e << "\n";
    fout.close();
}

