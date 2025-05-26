#include "speletaju_rezultati.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

void to_json(json& j, const SpeletajuRezultati& p) {
    j = json{{"username", p.username}, {"wins", p.wins}, {"losses", p.losses}, {"totalPoints", p.totalPoints}};
}

void from_json(const json& j, SpeletajuRezultati& p) {
    j.at("username").get_to(p.username);
    j.at("wins").get_to(p.wins);
    j.at("losses").get_to(p.losses);
    j.at("totalPoints").get_to(p.totalPoints);
}

void saveRankings(const std::vector<SpeletajuRezultati>& rankings, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        json j = rankings;
        file << j.dump(4);
        file.close();
    }
}

std::vector<SpeletajuRezultati> loadRankings(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<SpeletajuRezultati> rankings;
    if (file.is_open()) {
        json j;
        file >> j;
        rankings = j.get<std::vector<SpeletajuRezultati>>();
        file.close();
    }
    return rankings;
}

void updateSpeletajuRezultati(std::vector<SpeletajuRezultati>& rankings, const std::string& username, bool won, int points) {
    auto it = std::find_if(rankings.begin(), rankings.end(), [&](const SpeletajuRezultati& p) {
        return p.username == username;
    });

    if (it != rankings.end()) {
        if (won) it->wins++;
        else it->losses++;
        it->totalPoints += points;
    } else {
        rankings.push_back({username, won ? 1 : 0, won ? 0 : 1, points});
    }
}

void displayRankings(const std::vector<SpeletajuRezultati>& rankings) {
    std::vector<SpeletajuRezultati> sorted = rankings;
    std::sort(sorted.begin(), sorted.end(), [](const SpeletajuRezultati& a, const SpeletajuRezultati& b) {
        return a.totalPoints > b.totalPoints;
    });

    std::cout << "Ranku Tabula" << std::endl;
    for (const auto& p : sorted) {
        std::cout << p.username << " - Uzvaras: " << p.wins << ", Zaudējumi: " << p.losses
                  << ", Punkti: " << p.totalPoints << std::endl;
    }
}
