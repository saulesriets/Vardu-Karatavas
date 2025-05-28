#pragma once
#include "json.hpp"
#include "Game.h"
#include "user.h"
#include "speletaju_rezultati.hpp"
#include <string>

class Speletajs : public User {
public:
    Speletajs(std::string vards, char l, bool s) : User(vards, l, s) {}
    void execute();
    void Spelesana();
};


