#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "structs.h"

void SetPic(ftxui::Element& element, uint8_t code);

void SetType(std::string& type, uint8_t weather_code);


void DrawDayPart(ftxui::Element& element, const weather_info& weather,
                 const std::string& day_part);

void DrawDay(ftxui::Element& element, const day_info& weather);
