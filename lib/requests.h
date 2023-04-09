#pragma once

#include <map>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "structs.h"

using json = nlohmann::json;

coord GetCoords(const std::string& city_name);

std::vector<day_info> GetWeather(const coord& coords, uint8_t forecast_days);

void UpdateAllWeather(std::map<std::string, city_info>& cities, uint8_t days);

