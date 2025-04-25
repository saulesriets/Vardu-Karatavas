#ifndef MORE_HPP
#define MORE_HPP

#include <vector>
#include <string>
using namespace std;

// Declare the variables with extern
extern vector<string> fruit;
extern vector<string> vegetable;
extern vector<string> dairy;
extern char guess;
extern int lives;

// Declare functions
void Dashes(string word);
void DashesWithGuesses(string word, string guesses);
void Spele();

#endif
