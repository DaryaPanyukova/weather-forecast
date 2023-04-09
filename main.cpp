#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"


#include <chrono>
#include <thread>
#include <atomic>
#include <string>

#include <map>
#include <omfl/parser.h>


using json = nlohmann::json;


struct coord {
    double latitude;
    double longitude;

    coord(double latitude0, double longitude0) {
        latitude = latitude0;
        longitude = longitude0;
    }

    coord(const coord& coords) = default;

    coord() = default;
};

struct weather_info {
    uint8_t weather_code;
    int8_t temp;
    uint8_t windspeed;
    int16_t winddirection;
    uint8_t precipitation;
    uint8_t precipitation_probability;
};

struct day_info {
    weather_info morning, day, evening, night;
    std::string date;
};

struct city_info {
    coord coords;
    weather_info cur_weather;
    std::vector<day_info> weather;
};


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


void SetPic(ftxui::Element& element, uint8_t code) {
    if (code == 0 || code == 1) {
        element = ftxui::vbox({
                                      ftxui::text("     \\   /        "),
                                      ftxui::text("      .-.          "),
                                      ftxui::text("   ― (   ) ―      "),
                                      ftxui::text("      `-’          "),
                                      ftxui::text("     /   \\        ")
                              }) | ftxui::flex;
    } else if (code == 2) {
        element = ftxui::vbox({
                                      ftxui::text("   .( ).           "),
                                      ftxui::text("  (_._)_) .(  )    "),
                                      ftxui::text("         (_.)_._)  ")
                              }) | ftxui::flex;
    } else if (code == 3) {
        element = ftxui::vbox({
                                      ftxui::text("          .--.     "),
                                      ftxui::text("      .-( --- ).   "),
                                      ftxui::text("    (___.__)__)    ")
                              }) | ftxui::flex;
    } else if (code == 45 || code == 48) {
        element = ftxui::vbox({
                                      ftxui::text("  -_   -  _ --     "),
                                      ftxui::text("    _  -  _- -_    "),
                                      ftxui::text("   - _-  _ _- -    ")
                              }) | ftxui::flex;
    } else if (code == 51 || code == 53 || code == 55 || code == 56 ||
               code == 57 || code == 61 || code == 63 || code == 66) {
        element = ftxui::vbox({
                                      ftxui::text("  .( ).            "),
                                      ftxui::text(" (_._)_) (  )      "),
                                      ftxui::text(" ' '   (_.)_._)    "),
                                      ftxui::text("  '   '   '  '     "),
                                      ftxui::text("        '  '       ")
                              }) | ftxui::flex;
    } else if (code == 65 || code == 67 || code == 80 ||
               code == 81 || code == 82) {
        element = ftxui::vbox({
                                      ftxui::text("         .--.      "),
                                      ftxui::text("     .-( --- ).    "),
                                      ftxui::text("    (___.__)__)    "),
                                      ftxui::text("     / /  /  /     "),
                                      ftxui::text("    / /  /  /      ")
                              }) | ftxui::flex;
    } else if (code == 71 || code == 73) {
        element = ftxui::vbox({
                                      ftxui::text("   .( ).           "),
                                      ftxui::text("  (_._)_) (  )     "),
                                      ftxui::text("  *   * (_.)_._)   "),
                                      ftxui::text("     *    *   *    "),
                                      ftxui::text("        *   *      ")
                              }) | ftxui::flex;
    } else if (code == 75 || code == 85 || code == 86) {
        element = ftxui::vbox({
                                      ftxui::text("         .--.      "),
                                      ftxui::text("     .-( --- ).    "),
                                      ftxui::text("    (___.__)__)    "),
                                      ftxui::text("    * *  * * *     "),
                                      ftxui::text("   * * * * * *     ")
                              }) | ftxui::flex;
    } else if (code == 77) {
        element = ftxui::vbox({
                                      ftxui::text("         .--.      "),
                                      ftxui::text("     .-( --- ).    "),
                                      ftxui::text("    (___.__)__)    "),
                                      ftxui::text("     o  o   o      "),
                                      ftxui::text("     o o  o  o     "),
                              }) | ftxui::flex;
    } else if (code == 95) {
        element = ftxui::vbox({
                                      ftxui::text("         .--.      "),
                                      ftxui::text("     .-( --- ).    "),
                                      ftxui::text("    (___.__)__)    "),
                                      ftxui::text("      _/  _/       "),
                                      ftxui::text("     /   /         ")
                              }) | ftxui::flex;
    } else if (code == 96 || code == 99) {
        element = ftxui::vbox({
                                      ftxui::text("         .--.      "),
                                      ftxui::text("     .-( --- ).    "),
                                      ftxui::text("    (___.__)__)    "),
                                      ftxui::text("    o  _/  _/      "),
                                      ftxui::text("      / o /  o     "),
                              }) | ftxui::flex;
    }
}

void SetType(std::string& type, uint8_t weather_code) {
    switch (weather_code) {
        case 0:
            type = "Clear sky             ";
            break;
        case 1:
            type = "Mainly clear          ";
            break;
        case 2:
            type = "Partly cloudy         ";
            break;
        case 3:
            type = "Overcast              ";
            break;
        case 45:
            type = "Fog                   ";
            break;
        case 48:
            type = "Depositing rime fog   ";
            break;
        case 51:
            type = "Light drizzle         ";
            break;
        case 53:
            type = "Moderate drizzle      ";
            break;
        case 55:
            type = "Dense drizzle         ";
            break;
        case 56:
            type = "Light freezing drizzle";
            break;
        case 57:
            type = "Dense freezing drizzle";
            break;
        case 61:
            type = "Slight rain           ";
            break;
        case 63:
            type = "Moderate rain         ";
            break;
        case 65:
            type = "Heavy rain            ";
            break;
        case 66:
            type = "Light freezing rain   ";
            break;
        case 67:
            type = "Heavy freezing rain   ";
            break;
        case 71:
            type = "Slight snow fall      ";
            break;
        case 73:
            type = "Moderate snow fall    ";
            break;
        case 75:
            type = "Heavy snow fall       ";
            break;
        case 77:
            type = "Snow grains           ";
            break;
        case 80:
            type = "Slight rain shower    ";
            break;
        case 81:
            type = "Moderate rain shower  ";
            break;
        case 82:
            type = "Violent rain shower   ";
            break;
        case 85:
            type = "Slight snow shower    ";
            break;
        case 86:
            type = "Heavy snow shower     ";
            break;
        case 95:
            type = "Thunderstorm          ";
            break;
        case 96:
            type = "Thunderstorm with hail";
            break;
        case 99:
            type = "Thunderstorm with hail";
            break;
    }
}


void DrawDayPart(ftxui::Element& element, const weather_info& weather,
                 const std::string& day_part) {

    ftxui::Element pic;
    SetPic(pic, weather.weather_code);

    std::string type;
    SetType(type, weather.weather_code);

    std::string direction = std::to_string(weather.winddirection);
    if (weather.winddirection >= 337.5 || weather.winddirection < 22.5) {
        direction = "N";
    } else if (weather.winddirection >= 22.5 && weather.winddirection < 67.5) {
        direction = "NE";
    } else if (weather.winddirection >= 67.5 && weather.winddirection < 112.5) {
        direction = "E";
    } else if (weather.winddirection >= 112.5 &&
               weather.winddirection < 157.5) {
        direction = "SE";
    } else if (weather.winddirection >= 157.5 &&
               weather.winddirection < 202.5) {
        direction = "S";
    } else if (weather.winddirection >= 202.5 &&
               weather.winddirection < 247.5) {
        direction = "SW";
    } else if (weather.winddirection >= 247.5 &&
               weather.winddirection < 292.5) {
        direction = "W";
    } else if (weather.winddirection >= 292.5 &&
               weather.winddirection < 337.5) {
        direction = "NW";
    }

    element = window(ftxui::text(day_part) | ftxui::hcenter | ftxui::bold,
                     ftxui::hbox({
                                         pic | ftxui::hcenter | ftxui::vcenter,
                                         ftxui::vbox({ftxui::text(type) |
                                                      ftxui::bold, ftxui::text(
                                                 std::to_string(weather.temp) +
                                                 " °С"),
                                                      ftxui::text(direction +
                                                                  " | " +
                                                                  std::to_string(
                                                                          weather.windspeed) +
                                                                  " km/h"),
                                                      ftxui::text(
                                                              std::to_string(
                                                                      weather.precipitation) +
                                                              " mm | " +
                                                              std::to_string(
                                                                      weather.precipitation_probability) +
                                                              " %")}) |
                                         ftxui::vcenter
                                 }));
}

void DrawDay(ftxui::Element& element, const day_info& weather) {
    ftxui::Element morning, day, evening, night;
    DrawDayPart(morning, weather.morning, "morning");
    DrawDayPart(day, weather.day, "day");
    DrawDayPart(evening, weather.evening, "evening");
    DrawDayPart(night, weather.night, "night");

    element = vbox(ftxui::text("date: " + weather.date),
                   hbox(morning, day, evening, night));
}


int main(int argc, char** argv) {
    const std::filesystem::path path(argv[1]);
    // ----------- parsing config -----------
  //  const std::filesystem::path path = "D:\\ITMOlabs\\basics_c++\\labwork-10-DaryaPanyukova\\example\\config.omfl";
    omfl::Parser parser(path);
    parser.Parse();

    // --------- getting cities names --------
    auto cities_list = parser.Get("city");

    // ------- getting number of days -------
    uint8_t days = parser.Get("days")->AsInt();
    uint8_t frequency = parser.Get("frequency")->AsIntOrDefault(
            1); // update every frequency minutes

    // --------- getting cities weather --------
    std::map<std::string, city_info> cities;
    for (size_t i = 0; i < cities_list->size(); ++i) {
        std::string city_name = (*cities_list)[i].AsString();
        cities[city_name].coords = GetCoords(city_name);
        cities[city_name].weather = GetWeather(cities[city_name].coords, days);
    }


    // ------------ constants ----------
    const uint8_t kMaxDaysOnScreen = 3;
    const uint8_t kMaxDays = 16;


    // ------------ drawing ------------
    auto cur_city = cities.begin();
    uint8_t start = 0;
    uint8_t finish = std::min(days, kMaxDaysOnScreen);

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    auto renderer = ftxui::Renderer(
            [&start, &finish, &cur_city] {
                ftxui::Elements children;
                children.push_back(ftxui::text(cur_city->first) | ftxui::bold);
                for (size_t i = start; i < finish; ++i) {
                    ftxui::Element day;
                    DrawDay(day, cur_city->second.weather[i]);
                    children.push_back(day);
                }
                ftxui::Element ans = vbox(std::move(children));
                return ans;
            });

    auto component = CatchEvent(renderer, [&](ftxui::Event event) {
        if (event == ftxui::Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        } else if (event == ftxui::Event::Character("+")) {
            days = days + 1 < kMaxDays ? days + 1 : kMaxDays;
            if (cities.begin()->second.weather.size() < days) {
                UpdateAllWeather(cities, days);
            }
            if (finish < kMaxDaysOnScreen) {
                finish = days;
            }
            return true;
        } else if (event == ftxui::Event::Character("-")) {
            days = days - 1 > 1 ? days - 1 : 1;
            finish = days < finish? days:finish;
            start = finish -  kMaxDaysOnScreen > 0 ? finish -  kMaxDaysOnScreen : 0;
            return true;
        } else if (event.mouse().button == ftxui::Mouse::WheelDown ||
                   event == ftxui::Event::ArrowDown) {
            finish = days < finish + 1? days : finish + 1;
            start = finish - kMaxDaysOnScreen > 0? finish - kMaxDaysOnScreen : 0;
            return true;
        } else if (event.mouse().button == ftxui::Mouse::WheelUp ||
                   event == ftxui::Event::ArrowUp) {
            start = start - 1 > 0? start - 1 : 0;
            finish = start + kMaxDaysOnScreen < days? start + kMaxDaysOnScreen : days;
            return true;
        } else if (event.character() == "n" ||
                   event == ftxui::Event::ArrowRight) {
            ++cur_city;
            if (cur_city == cities.end()) {
                cur_city = cities.begin();
            }
            return true;
        } else if (event.character() == "p" ||
                   event == ftxui::Event::ArrowLeft) {
            if (cur_city == cities.begin()) {
                cur_city = cities.end();
            }
            cur_city--;
            return true;
        }
        return false;
    });


    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(frequency));
            screen.Post([&] {
                UpdateAllWeather(cities, days);
            });
            screen.Post(ftxui::Event::Custom);
        }
    });

    screen.Loop(component);
    refresh_ui_continue = false;
    refresh_ui.join();

    return EXIT_SUCCESS;
}


