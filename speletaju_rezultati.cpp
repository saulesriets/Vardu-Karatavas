#include "speletaju_rezultati.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

void to_json(json& j, const SpeletajuRezultati& s) {
    j = json{{"lietotajvards", s.lietotajvards}, {"uzvaras", s.uzvaras}, {"zaudejumi", s.zaudejumi}, {"punktiKopa", s.punktiKopa}};
}

void from_json(const json& j, SpeletajuRezultati& s) {
    j.at("lietotajvards").get_to(s.lietotajvards);
    j.at("uzvaras").get_to(s.uzvaras);
    j.at("zaudejumi").get_to(s.zaudejumi);
    j.at("punktiKopa").get_to(s.punktiKopa);
}

void saglabatRezultatus(const std::vector<SpeletajuRezultati>& rezultati, const std::string& fails) {
    std::ofstream izvade(fails);
    if (izvade.is_open()) {
        json j = rezultati;
        izvade << j.dump(4);
        izvade.close();
    }
}

std::vector<SpeletajuRezultati> nolasitRezultatus(const std::string& fails) {
    std::ifstream ievade(fails);
    std::vector<SpeletajuRezultati> rezultati;
    if (ievade.is_open()) {
        json j;
        ievade >> j;
        rezultati = j.get<std::vector<SpeletajuRezultati>>();
        ievade.close();
    }
    return rezultati;
}

void atjaunotRezultatu(std::vector<SpeletajuRezultati>& rezultati, const std::string& lietotajvards, bool uzvareja, int ieguutiePunkti) {
    auto atrasts = std::find_if(rezultati.begin(), rezultati.end(), [&](const SpeletajuRezultati& s) {
        return s.lietotajvards == lietotajvards;
    });

    if (atrasts != rezultati.end()) {
        if (uzvareja) atrasts->uzvaras++;
        else atrasts->zaudejumi++;
        atrasts->punktiKopa += ieguutiePunkti;
    } else {
        rezultati.push_back({lietotajvards, uzvareja ? 1 : 0, uzvareja ? 0 : 1, ieguutiePunkti});
    }
}

void paraditRezultatus(const std::vector<SpeletajuRezultati>& rezultati) {
    std::vector<SpeletajuRezultati> sakartoti = rezultati;
    std::sort(sakartoti.begin(), sakartoti.end(), [](const SpeletajuRezultati& a, const SpeletajuRezultati& b) {
        return a.punktiKopa > b.punktiKopa;
    });

    std::cout << "===== Rangu Tabula =====" << std::endl;
    for (const auto& s : sakartoti) {
        std::cout << s.lietotajvards << " - Uzvaras: " << s.uzvaras
                  << ", Zaudējumi: " << s.zaudejumi
                  << ", Punkti: " << s.punktiKopa << std::endl;
    }
}
