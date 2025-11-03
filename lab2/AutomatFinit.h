#ifndef AUTOMATFINIT_H
#define AUTOMATFINIT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <string>

class AutomatFinit {
private:
    // multimea starilor
    std::set<std::string> Q;
    // alfabetul
    std::set<char> Sigma;
    // tranzitii
    std::map<std::pair<std::string,char>, std::string> Delta;
    // starea initiala
    std::string q0;
    // starile finale
    std::set<std::string> F;
    // automatul e determinist
    bool determinist = true;

public:
    void citireFisier(const std::string& numeFisier);
    void citireTastatura();

    void afisare() const;

    bool verificaSecventa(const std::string& input);
    std::string prefixAcceptat(const std::string& input);

private:
    static std::string trim(std::string s);
    void parseLista(std::string s, std::set<std::string>& S);
    void parseAlfabet(std::string s);
    void parseTranzitie(const std::string& linie);
};

#endif // AUTOMATFINIT_H
