#pragma once

#include <string>
#include <vector>

class Category
{
public:
    std::string catName;
    std::vector<std::string> words;

    Category(const std::string& catName, const std::vector<std::string>& words);

    std::string getRandomWord() const;
};
