#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;
using namespace std;

char guess;
int random = 0, wordLengthMax = 0, wordLengthMin = 0, lives = 5;
string word;
vector<string> wordList;
vector<string> wordsInLvl;
// rāda strīpiņas
void Dashes(string word)
{
    vector<char> visual(word.size(), '_');
    for (int i = 0; i < word.size(); i++)
    {
        cout << visual[i] << " ";
    }
    cout << endl;
}
// rāda strīpiņas ar uzminētajiem burtiem
void DashesWithGuesses(string word, string guesses)
{
    vector<char> visual(word.size(), '_');
    for (int i = 0; i < word.size(); i++)
    {
        if (guesses.find(word[i]) != string::npos)
        {
            visual[i] = word[i];
        }
        cout << visual[i] << " ";
    }
    cout << endl;
}
// savāc attiecīgā garuma vārdus
string DiffLvl(int wordLengthMax, int wordLengthMin, const json &data)
{
    for (const auto &[category, words] : data.items())
    {
        wordList.insert(wordList.end(), words.begin(), words.end());
    }
    for (int i = 0; i < wordList.size(); i++)
    {
        if (wordList[i].size() >= wordLengthMin && wordList[i].size() <= wordLengthMax)
        {
            wordsInLvl.push_back(wordList[i]);
        }
    }
    if (wordsInLvl.empty())
    {
        cout << "No words found. Choose a different level." << endl;
    }
    random = rand() % wordsInLvl.size();
    word = wordsInLvl[random];
    return word;
}

int main()
{
    srand(time(0));
    string userInput;

    cout << "Wellcome! Would you like to choose a word category or difficulty level? (cat or diff)" << endl;
    cin >> userInput;

    ifstream file("Dictionary.json");
    if (!file.is_open())
    {
        cerr << "Failed to open Dictionary.json" << endl;
        return 1;
    }
    json data;
    file >> data;
    file.close();
    // ja izvēlsa pēc kategorijas
    if (userInput == "cat")
    {
        cout << "Available categories:\n";
        for (auto &[categoryName, items] : data.items())
        {
            cout << "- " << categoryName << "\n";
        }
        cout << "- random\n";
        cout << "\nEnter your choice: ";
        cin >> userInput;
        // ja izvēle sakrīt ar kādu no kategorijām
        if (data.contains(userInput))
        {   //kategorijas saturu saliek vektorā no kura izvēlēties vārdu
            wordList = data[userInput].get<vector<string>>();
            random = rand() % wordList.size();
            word = wordList[random];
        }
        // ja izvēlas random, savāc visus vārdus
        else if (userInput == "random")
        {
            for (const auto &[category, words] : data.items())
            {
                wordList.insert(wordList.end(), words.begin(), words.end());
            }
            random = rand() % wordList.size();
            word = wordList[random];
        }
        else
        {
            std::cout << "Invalid category" << std::endl;
            return 1;
        }
    }
    // ja izvēlas grūtības pakāpi
    else if (userInput == "diff")
    {
        cout << "Enter your choice (easy, medium, hard): ";
        cin >> userInput;
        if (userInput == "easy")
        {
            wordLengthMax = 5;
            wordLengthMin = 1;
            DiffLvl(wordLengthMax, wordLengthMin, data);
        }
        else if (userInput == "medium")
        {
            wordLengthMax = 7;
            wordLengthMin = 5;
            DiffLvl(wordLengthMax, wordLengthMin, data);
        }
        else if (userInput == "hard")
        {
            wordLengthMax = 50;
            wordLengthMin = 7;
            DiffLvl(wordLengthMax, wordLengthMin, data);
        }
        else
        {
            cout << "Invalid choice" << endl;
            return 1;
        }
    }

    cout << word + " (debug)" << endl; // debug
    cout << "Guess the fruit! \nYou have 5 lives!" << endl;
    Dashes(word);
    auto timeStart = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point timeEnd;

    string correctGuesses = "";
    string guessedLetters = "";
    double score = 0;
    bool allGuessed = false;
    // kamēr ir dzīvības, min burtus
    while (lives > 0)
    {
        cout << "Enter a letter: ";
        cin >> guess;
        guess = tolower(guess);
        bool found = false;
        for (int i = 0; i < word.size(); i++)
        {
            if (tolower(word[i]) == guess)
            {
                found = true;
                if (correctGuesses.find(guess) == string::npos)
                {
                    correctGuesses += guess;
                }
            }
        }
        if (found)
        {
            cout << guess << " is in the word!" << endl;
        }
        else
        {
            lives--;
            cout << "Wrong guess! Lives left: " << lives << endl;
        }
        DashesWithGuesses(word, correctGuesses);
        allGuessed = true;
        for (char c : word)
        {
            if (correctGuesses.find(tolower(c)) == string::npos)
            {
                allGuessed = false;
                break;
            }
        }
        if (allGuessed)
        {
            timeEnd = std::chrono::high_resolution_clock::now();
            break;
        }
        if (lives == 0)
        {
            cout << "Game over! The word was: " << word << endl;
            timeEnd = std::chrono::high_resolution_clock::now();
        }
    }
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();
    cout << "Time taken: " << duration << " seconds" << endl; // debug
    if (allGuessed)                                           // ja vārds uzminēts
    {
        score = (lives + word.size() + (60 - duration) * 10);
    }
    else if (lives == 0) // ja kkas ir uzminēts
    {
        score = correctGuesses.size() * 10;
    }
    cout << "Your score is: " << score << endl;
    return 0;
}
//////