#include <chrono>
#include <thread>
#include <string>
#include <map>

#include "omfl/parser.h"
#include "lib/draw.h"
#include "lib/requests.h"



int main(int argc, char** argv) {
    const std::filesystem::path path(argv[1]);
    // ----------- parsing config -----------
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
    refresh_ui.join();

    return EXIT_SUCCESS;
}

