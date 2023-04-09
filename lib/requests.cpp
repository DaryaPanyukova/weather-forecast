#include "requests.h"

coord GetCoords(const std::string& city_name) {
    cpr::Response city_resp = cpr::Get(
            cpr::Url{"https://api.api-ninjas.com/v1/city"},
            cpr::Header{{"X-Api-Key",
                         "K8cmyCUTIWUVPJjhonDKoQ==F8tKH15dpVVIbzo2"}},
            cpr::Parameters{{"name", city_name}});

    if (city_resp.status_code != 200 || city_resp.text == "[]") {
        throw std::runtime_error("Invalid response");
    }
    json city_json = json::parse(city_resp.text);

    double latitude = city_json[0]["latitude"];
    double longitude = city_json[0]["longitude"];
    return coord{latitude, longitude};
}

std::vector<day_info> GetWeather(const coord& coords, uint8_t forecast_days) {
    // запрос к open meteo
    cpr::Response weather_resp = cpr::Get(
            cpr::Url{"https://api.open-meteo.com/v1/forecast"},
            cpr::Parameters{{"latitude",      std::to_string(coords.latitude)},
                            {"longitude",     std::to_string(coords.longitude)},
                            {"forecast_days", std::to_string(forecast_days)},
                            {"hourly",        "temperature_2m,windspeed_10m,winddirection_10m,precipitation,precipitation_probability,weathercode"}});


    if (weather_resp.status_code != 200 || weather_resp.text == "[]") {
        throw std::runtime_error("Invalid response");
    }
    json j = json::parse(weather_resp.text);


    std::vector<day_info> weather;
    weather.reserve(forecast_days);
    for (size_t i = 0; i < forecast_days; ++i) {
        size_t morning_hour = 24 * i + 9;
        size_t day_hour = 24 * i + 13;
        size_t evening_hour = 24 * i + 18;
        size_t night_hour = 24 * i + 23;

        weather_info morning{j["hourly"]["weathercode"][morning_hour],
                             j["hourly"]["temperature_2m"][morning_hour],
                             j["hourly"]["windspeed_10m"][morning_hour],
                             j["hourly"]["winddirection_10m"][morning_hour],
                             j["hourly"]["precipitation"][morning_hour],
                             j["hourly"]["precipitation_probability"][morning_hour]};

        weather_info day{j["hourly"]["weathercode"][day_hour],
                         j["hourly"]["temperature_2m"][day_hour],
                         j["hourly"]["windspeed_10m"][day_hour],
                         j["hourly"]["winddirection_10m"][day_hour],
                         j["hourly"]["precipitation"][day_hour],
                         j["hourly"]["precipitation_probability"][day_hour]};

        weather_info evening{j["hourly"]["weathercode"][evening_hour],
                             j["hourly"]["temperature_2m"][evening_hour],
                             j["hourly"]["windspeed_10m"][evening_hour],
                             j["hourly"]["winddirection_10m"][evening_hour],
                             j["hourly"]["precipitation"][evening_hour],
                             j["hourly"]["precipitation_probability"][evening_hour]};

        weather_info night{j["hourly"]["weathercode"][night_hour],
                           j["hourly"]["temperature_2m"][night_hour],
                           j["hourly"]["windspeed_10m"][night_hour],
                           j["hourly"]["winddirection_10m"][night_hour],
                           j["hourly"]["precipitation"][night_hour],
                           j["hourly"]["precipitation_probability"][night_hour]};

        std::string date = (static_cast<std::string>(j["hourly"]["time"][day_hour])).substr(
                0, 10);
        weather.push_back({morning, day, evening, night, date});
    }

    return weather;
}

void UpdateAllWeather(std::map<std::string, city_info>& cities, uint8_t days) {
    for (auto& elem: cities) {
        elem.second.weather = GetWeather(elem.second.coords, days);
    }
}

