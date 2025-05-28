#include "speletajs.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "user.h"


using namespace std;
using json = nlohmann::json;

void Speletajs::execute() {
    int sIzv;
    do{
        cout << "Izvēlies darbību: " << endl;
        cout << "1 - Spēlēt" << endl;
        cout << "2 - Skatīt ranku tabulu" << endl;
        cout << "3 - Aizvērt" << endl;

        while (true) {
            cout << "Ievadi izvēli (ciparu): ";
            if (cin >> sIzv) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << "Lūdzu, ievadi veselu skaitli!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (sIzv) {
        case 1: {
            Spelesana();
            break;
        }
        case 2: {
            auto rezultati = nolasitRezultatus("rezultati.json");
            paraditRezultatus(rezultati);
            break;
        }
        case 3:{
            cout << "Izvēle beigt!" << endl;
            break;
        }
        default:{
            cout << "Izvēlieties no dotajām izvēlēm!" << endl;
            break;
        }
    }
  }while (sIzv != 3);
}

void Speletajs::Spelesana(){
    srand(time(0));
    ifstream file("Dictionary.json");
    if (!file.is_open()) {
        cerr << "Could not open Dictionary.json" << endl;
        return;
    }

    json data;
    file >> data;
    file.close();

    char playAgain;
    do {
        Game game(data);
        string input;

        cout << "Choose word by category or by difficulty (cat or diff): ";
        cin >> input;

        try {
            if (input == "cat") {
                cout << "Available categories:\n";
                for (auto &[name, _] : data.items()) cout << "- " << name << endl;
                cout << "- random\n";
                cout << "Enter category: ";
                cin >> input;
                game.chooseCategory(input);
            } else if (input == "diff") {
                cout << "Choose difficulty (easy, medium, hard): ";
                cin >> input;
                game.chooseByDifficulty(input);
            } else {
                cout << "Invalid option." << endl;
                continue;
            }

            game.play();
            game.saglabatZurnalu(getLietotajvards());


            bool uzvareja = game.getWinStatus();
            int punkti = game.getScore();

            auto rezultati = nolasitRezultatus("rezultati.json");
            atjaunotRezultatu(rezultati, getLietotajvards(), uzvareja, punkti);
            saglabatRezultatus(rezultati, "rezultati.json");
            paraditRezultatus(rezultati);

        } catch (exception &e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;

    } while (tolower(playAgain) == 'y');
}
