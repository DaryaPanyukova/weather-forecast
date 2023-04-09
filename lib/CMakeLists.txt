add_library(weather
        draw.h draw.cpp
        requests.h requests.cpp
        structs.h)
target_link_libraries(weather PRIVATE cpr::cpr)
target_link_libraries(weather PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(weather PRIVATE ftxui::dom ftxui::component ftxui::screen)