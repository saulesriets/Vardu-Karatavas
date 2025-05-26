#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "json.hpp"
#include "Category.h"

class Game
{
private:
    std::vector<Category> categories;
    std::string currentWord, correctGuesses; // minamais vārds un uzminētie burti
    // vērtības rezultātam
    int lives, remainingLives = 0, timeBonus = 0, finalScore = 0;
    size_t wordLength = 0;
    std::chrono::high_resolution_clock::time_point startTime, endTime;
    bool wordGuessed = false;

public:
    Game(const nlohmann::json &data);

    void chooseCategory(const std::string &userInput);
    void chooseByDifficulty(const std::string &level);
    void play();

    // funkcijas lai varētu padot rezultātus rangu tabulām
    int getRemainingLives() const;
    size_t getWordLength() const;
    int getTimeBonus() const;
    double getFinalScore() const;

     // vienkāršotās rezultātu tabulas funkcijas PAGAIDĀM
    bool getWinStatus() const;
    int getScore() const;

private:
    void displayDashes();
    void displayDashesWithGuesses();
    bool isComplete(); // pārbauda vai vārds uzminēts līdz galam
};
