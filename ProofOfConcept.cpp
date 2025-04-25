#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;
vector<string> fruit = {"watermelon", "strawberry", "pineapple", "papaya", "orange", "mango", "kiwi", "blueberry", "banana", "apple", "currant", "fig", "gooseberry", "date", "olive", "tangerine", "apricot", "boysenberry", "ackee", "durian"};
vector<string> vegetable = {"carrot", "potato", "tomato", "cucumber", "onion", "garlic", "broccoli", "cauliflower", "spinach", "lettuce", "bell pepper", "zucchini", "eggplant", "radish", "pumpkin", "beetroot", "celery", "asparagus", "artichoke"};
vector<string> dairy = {"milk", "cheese", "yogurt", "butter", "cream", "ice cream", "cottage cheese", "sour cream", "buttermilk", "ricotta", "feta", "mozzarella", "parmesan", "gouda", "brie", "camembert", "swiss cheese", "blue cheese", "cream cheese"};

// rāda strīpiņas
char guess;
int lives = 5;
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

int main()
{
    vector<string> choice = {"fruit", "vegetable", "dairy", "random"}; // vārdu kategorijas
    srand(time(0));
    int random = 0;
    string userInput, word;
    cout << "Enter your choice (fruit, vegetable, dairy, random): ";
    cin >> userInput;
    // random vārds no kategorijas
    if (userInput == "fruit")
    {
        random = rand() % fruit.size();
        word = fruit[random];
    }
    else if (userInput == "vegetable")
    {
        random = rand() % vegetable.size();
        word = vegetable[random];
    }
    else if (userInput == "dairy")
    {
        random = rand() % dairy.size();
        word = dairy[random];
    }
    else if (userInput == "random")
    {
        random = rand() % (fruit.size() + vegetable.size() + dairy.size());
        if (random < fruit.size())
        {
            word = fruit[random];
        }
        else if (random < fruit.size() + vegetable.size())
        {
            word = vegetable[random - fruit.size()];
        }
        else
        {
            word = dairy[random - fruit.size() - vegetable.size()];
        }
    }
    else
    {
        cout << "Invalid choice" << endl;
        return 1;
    }
    cout << word + " (debug)"<< endl; // debug
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
    if (allGuessed) // ja vārds uzminēts
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
