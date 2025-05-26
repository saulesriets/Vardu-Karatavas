#include "json.hpp"
#include "Game.h"
#include "speletaju_rezultati.hpp" //tabulai
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <limits>
#include <cstdlib>
#include <fstream>
using json = nlohmann::json;
using namespace std;

struct User
{
    string lietotajvards;
    char loma;
    bool LoggedIn;
};

User Login();
void Register();
string virknesParbaude(string zinojums);

User Login()
{
    string lietotajs, parole;
    bool LoggedIn = false;
    while (!LoggedIn)
    {
        lietotajs = virknesParbaude("Ievadiet lietotajvardu: ");
        ifstream Fails("Lietotaji.txt");
        if (!Fails)
        {
            cout << "Nevar atvērt failu!" << endl;
            break;
        }
        string failaLietotajs, failaParole, failasr;
        bool IrLietot = false, IrParole = false;
        while (Fails >> failaLietotajs >> failaParole >> failasr)
        {
            if (lietotajs == failaLietotajs)
            {
                IrLietot = true;
                parole = virknesParbaude("Ievadiet paroli: ");

                if (parole == failaParole)
                {
                    IrParole = true;
                }
                break;
            }
        }
        Fails.close();
        if (!IrLietot)
        {
            cout << "Šāds lietotājs neeksistē. Mēģini vēlreiz vai beidz (y/n)? ";
        }
        else if (!IrParole)
        {
            cout << "Nepareiza parole. Mēģini vēlreiz vai beidz (y/n)? ";
        }
        else
        {
            cout << "Veiksmīga pieteikšanās! Sveiks, " << lietotajs << "!" << endl;
            return User{lietotajs, failasr[0], true};
        }

        char beigt;
        cin >> beigt;
        if (beigt == 'n' || beigt == 'N')
        {
            cout << "Iziešana no sistēmas." << endl;
            return User{"", ' ', false};
            break;
        }
    }
    return User{"", ' ', false};
}

void Register()
{
    string lietotajs, parole;
    char rs;

    lietotajs = virknesParbaude("Ievadiet lietotajvardu: ");
    parole = virknesParbaude("Ievadiet paroli: ");
    cout << "Spēlētājs[s] vai Redaktors[r]: ";
    cin >> rs;
    rs = tolower(rs);
    while (rs != 's' && rs != 'r')
    {
        cout << "Spēlētājs[s] vai Redaktors[r]: ";
        cin >> rs;
        rs = tolower(rs);
    }

    ofstream Fails("Lietotaji.txt", ios::app);
    if (!Fails)
    {
        cout << "Nevar atvērt failu!\n";
        return;
    }

    Fails << lietotajs << " " << parole << " " << rs << endl;
    Fails.flush();
    Fails.close();

    cout << "Lietotājs veiksmīgi reģistrēts!" << endl;
}

string virknesParbaude(string zinojums)
{
    string virkne;
    do
    {
        cout << zinojums;
        getline(cin, virkne);
        if (virkne.empty())
            continue;

        virkne = regex_replace(virkne, regex("^ +| +$|( ) +"), "$1");
    } while (!regex_match(virkne, regex("^[a-zA-ZāčģēīķļņšūžĀČĢĒĪĶĻŅŠŪŽ]+$")));
    return virkne;
}

int main()
{
    int izvele;
    vector<string> darbibuSaraksts = {"1 - Login", "2 - Register", "3 - Aizvērt"};

    do
    {
        cout << "Izvēlies darbību: " << endl;
        for (const auto &darbiba : darbibuSaraksts)
        {
            cout << darbiba << endl;
        }
        while (true)
        {
            cout << "Ievadi izvēli (ciparu): ";
            if (cin >> izvele)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else
            {
                cout << "Lūdzu, ievadi veselu skaitli!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        switch (izvele)
        {
        case 1:
        {

            User tagadejais = Login();
            if (tagadejais.LoggedIn)
            {
                if (tagadejais.loma == 's')
                {
                    cout << "tu esi speletajs" << endl;
                    /////////
                    srand(time(0));
                    ifstream file("Dictionary.json");
                    if (!file.is_open())
                    {
                        cerr << "Could not open Dictionary.json" << endl;
                        continue;
                    }
                    json data;
                    file >> data;
                    file.close();

                    char playAgain;
                    do
                    {
                        Game game(data);
                        string input;

                        cout << "Choose word by category or by difficulty (cat or diff): ";
                        cin >> input;

                        try
                        {
                            if (input == "cat")
                            {
                                cout << "Available categories:\n";
                                for (auto &[name, _] : data.items())
                                    cout << "- " << name << endl;
                                cout << "- random\n";
                                cout << "Enter category: ";
                                cin >> input;
                                game.chooseCategory(input);
                            }
                            else if (input == "diff")
                            {
                                cout << "Choose difficulty (easy, medium, hard): ";
                                cin >> input;
                                game.chooseByDifficulty(input);
                            }
                            else
                            {
                                cout << "Invalid option." << endl;
                                continue;
                            }

                            game.play();

                            //rezultātu saglabāšana un atjaunošana
                            bool uzvareja = spele.getWinStatus(); //pievieno game klasei
                            int punkti = spele.getScore(); //pievieno game klasei
                            
                            vector<SpeletajuRezultati> rezultati = nolasitRezultatus("rezultati.json");
                            atjaunotRezultatu(rezultati, speletajs.lietotajvards, uzvareja, punkti);
                            saglabatRezultatus(rezultati, "rezultati.json");
                            
                            //parāda rezultātu
                            paraditRezultatus(rezultati);
                            
                        }
                        catch (exception &e)
                        {
                            cout << "Error: " << e.what() << endl;
                            continue;
                        }
                        cout << "Do you want to play again? (y/n): ";
                        cin >> playAgain;

                    } while (tolower(playAgain) == 'y');
                    cout << "DEBUG: User: " << tagadejais.lietotajvards
                         << ", LoggedIn: " << (tagadejais.LoggedIn ? "true" : "false") << endl;

                    // SPELETAJAM
                }
                else if (tagadejais.loma == 'r')
                {
                    cout << "tu esi redaktors" << endl;
                    // REDAKTORS
                    
                }
            }

            break;
        }
        case 2:
        {
            Register();
            cout << "Apsveicam, pieregistrejies!" << endl;
            break;
        }
        case 3:
        {
            cout << "Programma aizveras!" << endl;
            break;
        }
        default:
        {
            cout << "Izvēlieties no dotajā izvēlēm!" << endl;
            break;
        }
        }
    } while (izvele != 3);

    return 0;
}
