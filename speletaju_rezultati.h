#ifndef SPELETAJU_REZULTATI_HPP
#define SPELETAJU_REZULTATI_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Struktūra, lai glabātu spēlētāja rezultātus
struct SpeletajuRezultati {
    std::string lietotajvards;
    int uzvaras = 0;
    int zaudejumi = 0;
    int punktiKopa = 0;
};

// JSON konvertēšanas funkcijas
void to_json(nlohmann::json& j, const SpeletajuRezultati& s);
void from_json(const nlohmann::json& j, SpeletajuRezultati& s);

// Rezultātu failu darbības
void saglabatRezultatus(const std::vector<SpeletajuRezultati>& rezultati, const std::string& fails);
std::vector<SpeletajuRezultati> nolasitRezultatus(const std::string& fails);

// Rezultātu apstrāde
void atjaunotRezultatu(std::vector<SpeletajuRezultati>& rezultati, const std::string& lietotajvards, bool uzvareja, int ieguutiePunkti);
void paraditRezultatus(const std::vector<SpeletajuRezultati>& rezultati);

#endif // SPELETAJU_REZULTATI_HPP
