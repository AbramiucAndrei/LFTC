#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <regex>
#include <algorithm>

using namespace std;

set<string> keywords = {
    "floating", "inty", "stringus",
    "reading", "writing",
    "check_if", "else", "execute_while",
    "using", "namespace", "std", "int", "main",
    "return", "#include", "<iostream>"
};

set<char> separators = { ';', '(', ')', '{', '}', ',' };
set<string> operators = { "+", "-", "*", "/", "%", "==", "!=", "<", ">", "<=", ">=", ":=" };

regex IDENTIFIER_PATTERN("^[a-zA-Z_][a-zA-Z0-9_]*$");
regex CONST_PATTERN("^-?\\d+(\\.\\d+)?$");
regex STRING_PATTERN("^\".*\"$");
string LETTERS_DIGITS_DOT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.";

bool isCONST(const string& text) {
    return regex_match(text, CONST_PATTERN);
}

bool isIDENTIFIER(const string& text) {
    return regex_match(text, IDENTIFIER_PATTERN);
}

bool isSTRING(const string& text) {
    return regex_match(text, STRING_PATTERN);
}

bool isOPERATOR(const string& text) {
    return operators.find(text) != operators.end();
}

bool isSEPARATOR(const string& text) {
    return text.length() == 1 && separators.find(text[0]) != separators.end();
}

bool isKEYWORD(const string& text) {
    return keywords.find(text) != keywords.end();
}

vector<string> tokenize(const string& line) {
    vector<string> tokens;
    string current = "";

    for (int i = 0; i < line.length(); i++) {
        char ch = line[i];

        // strings: "..."
        if (ch == '"') {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            int endPos = line.find('"', i + 1);
            if (endPos != string::npos) {
                tokens.push_back(line.substr(i, endPos - i + 1));
                i = endPos;
                continue;
            }
        }

        // <...>
        if (ch == '<') {
            int endPos = line.find('>', i);
            if (endPos != string::npos && endPos > i + 1) {
                string innerContent = line.substr(i + 1, endPos - i - 1);
                if (innerContent.find_first_not_of(LETTERS_DIGITS_DOT) == string::npos) {
                    if (!current.empty()) {
                        tokens.push_back(current);
                        current = "";
                    }
                    tokens.push_back(line.substr(i, endPos - i + 1));
                    i = endPos;
                    continue;
                }
            }
        }

        // 2-ch operators
        if (i + 1 < line.length()) {
            string twoChar = line.substr(i, 2);
            if (isOPERATOR(twoChar)) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current = "";
                }
                tokens.push_back(twoChar);
                i++;
                continue;
            }
        }

        // separators
        if (separators.find(ch) != separators.end()) {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            tokens.push_back(string(1, ch));
        }
        // 1-ch operators
        else if (string("+-*/%<>").find(ch) != string::npos) {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            tokens.push_back(string(1, ch));
        }
        // ws
        else if (isspace(ch)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
        }
        else {
            current += ch;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

int main() {
    string input_file_path = "./sum.txt";
    string output_file_path = "./output.txt";

    ifstream inputFile(input_file_path);
    ofstream outputFile(output_file_path);

    if (!inputFile.is_open()) {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    vector<string> CONSTs, OPERATORI, SEPARATORI, CUVINTE_CHEIE, IDENTIFICATORI, STRINGURI;

    string linie;
    while (getline(inputFile, linie)) {
        vector<string> tokens = tokenize(linie);

        for (const string& e : tokens) {
            if (isKEYWORD(e)) {
                CUVINTE_CHEIE.push_back(e);
            }
            else if (isCONST(e)) {
                CONSTs.push_back(e);
            }
            else if (isSTRING(e)) {
                STRINGURI.push_back(e);
            }
            else if (isOPERATOR(e)) {
                OPERATORI.push_back(e);
            }
            else if (isSEPARATOR(e)) {
                SEPARATORI.push_back(e);
            }
            else if (isIDENTIFIER(e)) {
                IDENTIFICATORI.push_back(e);
            }
            else {
                cout << "Unknown! -" << e << "-\n";
                return 1;
            }
        }
    }

    outputFile << "CONSTs:" << endl;
    for (const string& c : CONSTs) {
        outputFile << c << endl;
    }

    outputFile << "OPERATORI:" << endl;
    for (const string& op : OPERATORI) {
        outputFile << op << endl;
    }

    outputFile << "SEPARATORI:" << endl;
    for (const string& sep : SEPARATORI) {
        outputFile << sep << endl;
    }

    outputFile << "CUVINTE_CHEIE:" << endl;
    for (const string& kw : CUVINTE_CHEIE) {
        outputFile << kw << endl;
    }

    outputFile << "IDENTIFICATORI:" << endl;
    for (const string& id : IDENTIFICATORI) {
        outputFile << id << endl;
    }

    outputFile << "STRINGURI:" << endl;
    for (const string& str : STRINGURI) {
        outputFile << str << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Done!" << endl;
    return 0;
}