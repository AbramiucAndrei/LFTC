#include "AutomatFinit.h"
using namespace std;

// CITIRE FISIER
void AutomatFinit::citireFisier(const string& numeFisier) {
    ifstream fin(numeFisier);
    if(!fin) {
        cerr << "Eroare la deschiderea fisierului!\n";
        return;
    }

    string linie;
    while (getline(fin, linie)) {
        if (linie.rfind("Q:",0)==0) {
            parseLista(linie.substr(2), Q);
        } else if (linie.rfind("Sigma:",0)==0) {
            parseAlfabet(linie.substr(6));
        } else if (linie.rfind("q0:",0)==0) {
            q0 = trim(linie.substr(3));
        } else if (linie.rfind("F:",0)==0) {
            parseLista(linie.substr(2), F);
        } else if (linie.rfind("Delta:",0)==0) {
            // urmeaza tranzitiile
            while (getline(fin, linie)) {
                if (linie.empty()) continue;
                parseTranzitie(linie);
            }
        }
    }
    fin.close();
}

// CITIRE tastatura
void AutomatFinit::citireTastatura() {
    int n;
    cout << "Numarul starilor: "; cin >> n;
    cout << "Introdu starile (ex: q0 q1 q2): ";
    for (int i=0;i<n;i++){ string s; cin>>s; Q.insert(s); }

    cout << "Introdu alfabetul (ex: a b 0 1 #) – scrie 'stop' pentru a opri: ";
    string simbol;
    while(cin>>simbol && simbol!="stop")
        Sigma.insert(simbol[0]);

    cout << "Stare initiala: "; cin >> q0;
    cout << "Numarul starilor finale: "; cin >> n;
    cout << "Starile finale: ";
    for (int i=0;i<n;i++){ string s; cin>>s; F.insert(s); }

    cout << "Numarul tranzitiilor: "; cin >> n;
    cout << "Introdu tranzitiile (forma: qX simbol qY):\n";
    for (int i=0;i<n;i++){
        string s1,s2; char c; cin >> s1 >> c >> s2;
        if (Delta.count({s1,c})) determinist=false;
        Delta[{s1,c}] = s2;
    }
}

// AFISARE Automat Finit
void AutomatFinit::afisare() const {
    cout << "Q = { "; for (auto &x:Q) cout<<x<<" "; cout<<"}\n";
    cout << "Sigma = { "; for (auto &x:Sigma) cout<<x<<" "; cout<<"}\n";
    cout << "q0 = " << q0 << "\n";
    cout << "F = { "; for (auto &x:F) cout<<x<<" "; cout<<"}\n";
    cout << "Tranzitii (Delta):\n";
    for (auto &t:Delta)
        cout << "Delta("<<t.first.first<<","<<t.first.second<<") = "<<t.second<<"\n";
    cout << "Tip automat: " << (determinist?"determinist":"nedeterminist") << "\n";
}

// VERIFICARE Secventa
bool AutomatFinit::verificaSecventa(const string& input) {
    if (!determinist) {
        cout << "Automatul nu este determinist!\n";
        return false;
    }
    string stare = q0;
    for (char c : input) {
        auto it = Delta.find({stare,c});
        if (it == Delta.end()) return false;
        stare = it->second;
    }
    return F.contains(stare);
}

//CEL MAI LUNG PREFIX
string AutomatFinit::prefixAcceptat(const string& input) {
    if (!determinist) {
        cout << "Automatul nu este determinist!\n";
        return "";
    }
    string stare = q0;
    string prefix="", longest="";
    for (char c : input) {
        auto it = Delta.find({stare,c});
        if (it == Delta.end()) break;
        stare = it->second;
        prefix += c;
        if (F.contains(stare)) longest = prefix;
    }
    return longest;
}

//FUNCTII UTILE
string AutomatFinit::trim(string s) {
    while(!s.empty() && isspace(s.front())) s.erase(s.begin());
    while(!s.empty() && isspace(s.back())) s.pop_back();
    return s;
}

void AutomatFinit::parseLista(string s, set<string>& S) {
    stringstream ss(s);
    string elem;
    while (getline(ss, elem, ',')) S.insert(trim(elem));
}

void AutomatFinit::parseAlfabet(string s) {
    stringstream ss(s);
    string elem;
    while (getline(ss, elem, ',')) {
        elem = trim(elem);
        if (!elem.empty()) Sigma.insert(elem[0]);
    }
}

void AutomatFinit::parseTranzitie(const string& linie) {
    stringstream ss(linie);
    string st1, sag, st2;
    getline(ss, st1, ',');
    char simbol;
    ss >> simbol;
    getline(ss, sag, '>');
    ss >> st2;
    st1 = trim(st1);
    st2 = trim(st2);
    if (Delta.count({st1,simbol})) determinist=false;
    Delta[{st1,simbol}] = st2;
}
