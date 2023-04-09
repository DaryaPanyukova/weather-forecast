#include "draw.h"

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
    } else {
        element = ftxui::text("No data");
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
        default:
            type = "No data";
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
                         ftxui::vbox({ftxui::text(type) | ftxui::bold,
                         ftxui::text(weather.null[1]?
                                "temp: no data": std::to_string(weather.temp) +" °С"),
                         ftxui::text(weather.null[3] || weather.null[2]?
                                "wind: no data" : (direction + " | " +
                                std::to_string(weather.windspeed) + " km/h")),
                         ftxui::text(weather.null[4] || weather.null[5]?
                                "precipitation: no data" :
                                (std::to_string (weather.precipitation) + " mm | " +
                              std::to_string(weather.precipitation_probability) +
                              " %"))
                         }) | ftxui::vcenter
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
