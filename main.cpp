#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include "json.hpp"
#include "Game.h"
using json = nlohmann::json;
using namespace std;

int main()
{
    srand(time(0));
    ifstream file("Dictionary.json");
    if (!file.is_open())
    {
        cerr << "Could not open Dictionary.json" << endl;
        return 1;
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
                return 1;
            }

            game.play();
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
            return 1;
        }
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;

    } while (tolower(playAgain) == 'y');

    cout << "Thanks for playing!\n";
    return 0;
}
