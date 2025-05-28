#pragma once
#include "json.hpp"
#include "user.h"
#include <string>

class Redaktors : public User {
public:
    Redaktors(std::string vards, char l, bool s) : User(vards, l, s) {}
    void execute();
    
};

void redaktoraVardMenu();
