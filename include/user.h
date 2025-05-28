#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    string lietotajvards;
    char loma;
    bool LoggedIn;

public:
    User(string vards = "", char l = ' ', bool login = false)
        : lietotajvards(vards), loma(l), LoggedIn(login) {}

    string getLietotajvards() const { return lietotajvards; }
    char getLoma() const { return loma; }
    bool isLoggedIn() const { return LoggedIn; }
};

#endif
