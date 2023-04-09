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


void SetWeather(const json& j, weather_info& weather, size_t hour) {
    if (j["hourly"]["weathercode"][hour].is_null()) {
        weather.null[0] = true;
    } else {
        weather.weather_code = j["hourly"]["weathercode"][hour];
    }

    if (j["hourly"]["temperature_2m"][hour].is_null()) {
        weather.null[1] = true;
    } else {
        weather.temp = j["hourly"]["temperature_2m"][hour];
    }

    if (j["hourly"]["windspeed_10m"][hour].is_null()) {
        weather.null[2] = true;
    } else {
        weather.windspeed = j["hourly"]["windspeed_10m"][hour];
    }


    if (j["hourly"]["winddirection_10m"][hour].is_null()) {
        weather.null[3] = true;
    } else {
        weather.winddirection = j["hourly"]["winddirection_10m"][hour];
    }

    if (j["hourly"]["precipitation"][hour].is_null()) {
        weather.null[4] = true;
    } else {
        weather.precipitation = j["hourly"]["precipitation"][hour];
    }

    if (j["hourly"]["precipitation_probability"][hour].is_null()) {
        weather.null[5] = true;
    } else {
        weather.precipitation_probability = j["hourly"]["precipitation_probability"][hour];
    }
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

        weather_info morning;
        SetWeather(j, morning, morning_hour);

        weather_info day;
        SetWeather(j, day, day_hour);

        weather_info evening;
        SetWeather(j, evening, evening_hour);

        weather_info night;
        SetWeather(j, night, night_hour);

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

