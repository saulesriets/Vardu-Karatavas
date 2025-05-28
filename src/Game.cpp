#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "json.hpp"  // šis vajadzīgs lai jsons strādātu
#include "Game.h"

using namespace std;


Game::Game(const nlohmann::json &data)
    {
        for (auto &[categoryName, wordArray] : data.items())
        {
            categories.emplace_back(categoryName, wordArray.get<vector<string>>());
        }
    }

    void Game::chooseCategory(const std::string& userInput)
    {
        // savāc visus vārdus
        if (userInput == "random")
        {
            vector<string> allWords;
            for (const auto &cat : categories)
            {
                allWords.insert(allWords.end(), cat.words.begin(), cat.words.end());
            }
            currentWord = allWords[rand() % allWords.size()]; // minamais vārds - random no saraksta
        }
        else
        {
            // savāc vārdus no izvēlētās kategorijas
            for (const auto &cat : categories)
            {
                if (cat.catName == userInput)
                {
                    currentWord = cat.getRandomWord();  // random no kategorijas
                    return;
                }
            }
            throw runtime_error("Invalid category");
        }
    }

    void Game::chooseByDifficulty(const std::string& level)
    {
        // savāc visus vārdus no visām kategorijām
        vector<string> allWords;
        for (const auto &cat : categories)
        {
            allWords.insert(allWords.end(), cat.words.begin(), cat.words.end());
        }
        // izvēlās vārdu līmeni
        int minLen = 1, maxLen = 50;
        if (level == "easy")
        {
            maxLen = 4;
        }
        else if (level == "medium")
        {
            minLen = 5;
            maxLen = 7;
        }
        else if (level == "hard")
        {
            minLen = 8;
        }
        // vektors ar pareizā garuma vārdiem
        vector<string> wordsByLength;
        for (const string &w : allWords)
        {
            if (w.size() >= minLen && w.size() <= maxLen)
                wordsByLength.push_back(w);
        }

        if (wordsByLength.empty())
            throw runtime_error("No words found for selected difficulty");
        currentWord = wordsByLength[rand() % wordsByLength.size()];
    }

    void Game::play()
    {
        lives = 5; // šito var ielikt pie līmeņa izvēles ar dažādām vērtībām?
        correctGuesses = ""; // uzkrāj pareizos burtus
        startTime = chrono::high_resolution_clock::now();

        cout << "Guess the word! You have " << lives << " lives.\n";
        displayDashes();
        cout << currentWord << "(debug)" << endl; // debug

        while (lives > 0)
        {
            cout << "Enter a letter: ";
            char guess;
            cin >> guess;
            guess = tolower(guess);

            bool found = false;
            // ja atrod vārdā burtu, to +pareizo burtu sarakstam
            for (char c : currentWord)
            {
                if (tolower(c) == guess)
                {
                    found = true;
                    if (correctGuesses.find(guess) == string::npos)
                        correctGuesses += guess;
                }
            }
            // ja burts vārdā nav, -dzīvība
            if (!found)
            {
                lives--;
                cout << "Wrong guess. Lives left: " << lives << endl;
            }
            else
            {
                cout << guess << " is in the word!" << endl;
            }

            displayDashesWithGuesses();
            if (isComplete())
                break;
        }
        // kkādi punkti rezultātam???
        endTime = chrono::high_resolution_clock::now();
        remainingLives = lives * 10; // atlikušo dzīvību punkti
        wordLength = currentWord.size() * 10; // grūtības punkti
        int duration = chrono::duration_cast<chrono::seconds>(endTime - startTime).count();// spēles ilgums
        timeBonus = (max(0, 90 - duration)) * 5; // laika bonuss (ja spēlē mazāk par 90 sekundēm)

        // rezultāta aprēķins
        wordGuessed = isComplete();
        if (wordGuessed)
        {
            finalScore = wordLength + timeBonus + remainingLives;
        }
        else
        {
            finalScore = correctGuesses.size() * 10;
        }

        cout << "Your score is: " << finalScore << endl;
        cout << "Remaining lives points: " << remainingLives << "\nTime bonus: " << timeBonus << "\nWord length points: " << wordLength << endl;

        if (lives > 0)
        {
            cout << "You guessed the word: " << currentWord << "!\n";
        }
        else
        {
            cout << "Game over! The word was: " << currentWord << endl;
        }
    }

    // funkcijas lai varētu padot rezultātus rangu tabulām
    int Game::getRemainingLives() const { return remainingLives; }
    size_t Game::getWordLength() const { return wordLength; }
    int Game::getTimeBonus() const { return timeBonus; }
    double Game::getFinalScore() const { return finalScore; }

    // strīpiņas
    void Game::displayDashes()
{
    for (char c : currentWord)
    {
        if (c == ' ')
            cout << "  ";
        else
            cout << "_ ";
    }
    cout << endl;
}

void Game::displayDashesWithGuesses()
{
    for (char c : currentWord)
    {
        if (c == ' ')
            cout << "  ";
        else if (correctGuesses.find(tolower(c)) != string::npos)
            cout << c << " ";
        else
            cout << "_ ";
    }
    cout << endl;
}

    // pārbauda vai vārds ir līdz galam uzminēts
    bool Game::isComplete()
    {
        for (char c : currentWord)
        {
            if (c == ' ') continue;
            if (correctGuesses.find(tolower(c)) == string::npos)
                return false;
        }
        return true;
    }

//tabulai pagaidu
bool Game::getWinStatus() const {
    return wordGuessed;
}

int Game::getScore() const {

    return static_cast<int>(finalScore);
}

void Game::saglabatZurnalu(const std::string& username) {
    std::ofstream outFile("Zurnals.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Nevar atvērt Zurnals.txt!" << std::endl;
        return;
    }

    int timeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    outFile << username << " | "
            << currentWord << " | "
            << "Lives: " << remainingLives << " | "
            << "Time: " << timeInSeconds << "s | "
            << "Score: " << finalScore << std::endl;
}
