#include "AutomatFinit.h"
using namespace std;

int main() {
    AutomatFinit af;
    int opt;

    do {
        cout << "\n=== MENIU ===\n";
        cout << "1. Citire automat din fisier (fix: afd_int.txt)\n";
        cout << "2. Citire automat de la tastatura\n";
        cout << "3. Afisare automat\n";
        cout << "4. Verifica daca o secventa este acceptata\n";
        cout << "5. Determina cel mai lung prefix acceptat\n";
        cout << "0. Iesire\n";
        cout << "Optiune: ";
        cin >> opt;
        cin.ignore();

        if (opt == 1) {
            const string numeFisier = "afd_int.txt";
            cout << "Se citeste automatul din fisierul fix: " << numeFisier << endl;
            af.citireFisier(numeFisier);
        }
        else if (opt == 2) {
            af.citireTastatura();
        }
        else if (opt == 3) {
            af.afisare();
        }
        else if (opt == 4) {
            string s;
            cout << "Secventa: ";
            cin >> s;
            cout << (af.verificaSecventa(s) ? "Acceptata" : "Respinsa") << "\n";
        }
        else if (opt == 5) {
            string s;
            cout << "Secventa: ";
            cin >> s;
            cout << "Cel mai lung prefix acceptat: " << af.prefixAcceptat(s) << "\n";
        }
    } while (opt != 0);
}
