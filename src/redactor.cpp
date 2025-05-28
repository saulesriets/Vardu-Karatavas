#include "redactor.h"
#include "user.h"
#include "speletaju_rezultati.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

void Redaktors::execute() {
    int rIzv;
    do{
        cout << "Izvēlies darbību: " << endl;
        cout << "1 - Rediģēt vārdnīcu" << endl;
        cout << "2 - Skatīt ranku tabulu" << endl;
        cout << "3 - Skatīt žurnālu" << endl;
        cout << "4 - Aizvērt" << endl;

        while (true) {
            cout << "Ievadi izvēli (ciparu): ";
            if (cin >> rIzv) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Lūdzu, ievadi veselu skaitli!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (rIzv) {
        case 1: {
            redaktoraVardMenu();
            break;
        }
        case 2: {
            auto rezultati = nolasitRezultatus("rezultati.json");
            paraditRezultatus(rezultati);
            break;
        }
        case 3: {
            std::ifstream inFile("Zurnals.txt");
            if (!inFile.is_open())
            {
                std::cerr << "Nevar atvērt Zurnals.txt!" << std::endl;
                return;
            }
            std::string line;
            while (std::getline(inFile, line))
            {
                std::cout << line << std::endl;
            }
            break;
        }
        case 4:{
            cout << "Izvēle beigt!" << endl;
            break;
        }
        default:{
            cout << "Izvēlieties no dotajām izvēlēm!" << endl;
            break;
        }
    }
  }while (rIzv != 4);

}

void redaktoraVardMenu() {
    int redaktorsIzvele;
    do {
        cout << "Izvēlies darbību: " << endl;
        cout << "1 - Skatīt vārdnīcu" << endl;
        cout << "2 - Pievienot vārdu/kategoriju" << endl;
        cout << "3 - Dzēst vārdu" << endl;
        cout << "4 - Dzēst kategoriju" << endl;
        cout << "5 - Aizvērt" << endl;

        while (true) {
            cout << "Ievadi izvēli (ciparu): ";
            if (cin >> redaktorsIzvele) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Lūdzu, ievadi veselu skaitli!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (redaktorsIzvele) {
        case 1: {
            ifstream file("Dictionary.json");
            json data;
            file >> data;
            file.close();

            cout << "Vārdnīca:" << endl;
            for (auto &[kategorija, vardi] : data.items()) {
                cout << kategorija << "| ";
                for (const auto &vards : vardi) cout << vards << " ";
                cout << endl;
            }
            break;
        }
        case 2: {
            string vards, kategorija;
            cout << "Ievadi vārdu: ";
            cin >> vards;
            cout << "Ievadi kategoriju: ";
            cin >> kategorija;

            ifstream file("Dictionary.json");
            json data;
            file >> data;
            file.close();

            data[kategorija].push_back(vards);

            ofstream outFile("Dictionary.json");
            outFile << data.dump(4);
            outFile.close();

            cout << "Vārds pievienots!" << endl;
            break;
        }
        case 3: {
            string kategorija, vards;
            cout << "Ievadi kategoriju, no kuras dzēst vārdu: ";
            cin >> kategorija;
            cout << "Ievadi vārdu, kuru dzēst: ";
            cin >> vards;

            ifstream file("Dictionary.json");
            json data;
            file >> data;
            file.close();

            auto it = find(data[kategorija].begin(), data[kategorija].end(), vards);
            if (it != data[kategorija].end()) {
                data[kategorija].erase(it);
                cout << "Vārds dzēsts!" << endl;
            } else {
                cout << "Vārds netika atrasts!" << endl;
            }

            ofstream outFile("Dictionary.json");
            outFile << data.dump(4);
            outFile.close();
            break;
        }
        case 4: {
            string kategorija;
            cout << "Ievadi kategoriju, kuru dzēst: ";
            cin >> kategorija;

            ifstream file("Dictionary.json");
            json data;
            file >> data;
            file.close();

            data.erase(kategorija);

            ofstream outFile("Dictionary.json");
            outFile << data.dump(4);
            outFile.close();

            cout << "Kategorija dzēsta!" << endl;
            break;
        }
        case 5:
            cout << "Izvēle beigt!" << endl;
            break;
        default:
            cout << "Izvēlieties no dotajām izvēlēm!" << endl;
            break;
        }

    } while (redaktorsIzvele != 5);
}
