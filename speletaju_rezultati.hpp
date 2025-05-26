#ifndef SPELETAJU_REZULTATI_HPP
#define SPELETAJU_REZULTATI_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct SpeletajuRezultati {
    std::string username;
    int wins = 0;
    int losses = 0;
    int totalPoints = 0;
};

// JSON 
void to_json(nlohmann::json& j, const SpeletajuRezultati& p);
void from_json(const nlohmann::json& j, SpeletajuRezultati& p);

void saveRankings(const std::vector<SpeletajuRezultati>& rankings, const std::string& filename);
std::vector<SpeletajuRezultati> loadRankings(const std::string& filename);
void updateSpeletajuRezultati(std::vector<SpeletajuRezultati>& rankings, const std::string& username, bool won, int points);
void displayRankings(const std::vector<SpeletajuRezultati>& rankings);

#endif // SPELETAJU_REZULTATI_HPP
