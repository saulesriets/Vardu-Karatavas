#include "Category.h"
#include <cstdlib>

Category::Category(const std::string& catName, const std::vector<std::string>& words)
    : catName(catName), words(words) {}

std::string Category::getRandomWord() const
{
    if (words.empty())
        return "";
    return words[rand() % words.size()];
}
