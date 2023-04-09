#pragma once

#include <string>
#include <vector>

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

    bool null[6] = {false};
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
