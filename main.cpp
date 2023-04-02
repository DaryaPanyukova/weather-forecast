#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include "json.hpp"
#include <map>
#include <omfl/parser.h>

using json = nlohmann::json;

struct coord {
    std::string latitude;
    std::string longitude;
    coord(double latitude0, double longitude0) {
        latitude = latitude0;
        longitude = longitude0;
    }
};

json GetWeather(coord& coords, std::string forecast_days) {
    // запрос к open meteo
    cpr::Response weather_resp = cpr::Get(
            cpr::Url{"https://api.open-meteo.com/v1/forecast"},
            cpr::Parameters{{"latitude",  coords.latitude},
                            {"longitude", coords.longitude},
                            {"forecast_days", forecast_days},
                            {"hourly",    "temperature_2m"}});
    json j = json::parse(weather_resp);
    return j;
}

void GetCoords (std::map <std::string, coord>& city_coords) {
    for (auto& elem: city_coords) {
        cpr::Response city_resp = cpr::Get(
                cpr::Url{"https://api.api-ninjas.com/v1/city"},
                cpr::Header{{"X-Api-Key",
                             "K8cmyCUTIWUVPJjhonDKoQ==F8tKH15dpVVIbzo2"}},
                cpr::Parameters{{"name", elem.first}});
        json city_json = json::parse(city_resp.text);

        double latitude = city_json[0]["latitude"];
        double longitude = city_json[0]["longitude"];
        elem.second = coord{latitude, longitude};
    }
}

int main() {
    const std::filesystem::path path = "D:\\ITMOlabs\\basics_c++\\labwork-10-DaryaPanyukova\\example\\config.omfl";
    omfl::Parser parser(path);
    parser.Parse();
// todo: если в конфиге нет какого-то параметра

    auto cities = parser.Get("city");
    size_t size = cities->size();
    std::map <std::string, coord> city_coords;
    for (size_t i = 0; i < size; ++i) {
        std::string city = (*cities)[i].AsString();
        city_coords[city] = {0, 0};
    }
    GetCoords(city_coords);

    std::string days = parser.Get("days")->AsString();




}