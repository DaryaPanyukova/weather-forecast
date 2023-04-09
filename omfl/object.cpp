#include "object.h"

bool omfl::Object::IsInt() const {
    throw std::runtime_error("isn't a value");
}

int32_t omfl::Object::AsInt() const {
    throw std::runtime_error("isn't a value");
}

int32_t omfl::Object::AsIntOrDefault(int32_t default_value) const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::IsFloat() const {
    throw std::runtime_error("isn't a value");
}

double omfl::Object::AsFloat() const {
    throw std::runtime_error("isn't a value");
}

double omfl::Object::AsFloatOrDefault(double default_value) const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::IsString() const {
    throw std::runtime_error("isn't a value");
}

std::string omfl::Object::AsString() const {
    throw std::runtime_error("isn't a value");
}

std::string omfl::Object::AsStringOrDefault(const std::string& default_value) const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::IsBool() const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::AsBool() const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::AsBoolOrDefault(bool default_value) const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Object::IsArray() const {
    throw std::runtime_error("isn't a value");
}

omfl::Object& omfl::Object::operator[](std::size_t index) {
    throw std::runtime_error("isn't a value");
}

size_t omfl::Object::size() const {
    throw std::runtime_error("isn't a value");
}

omfl::Object& omfl::Object::Get(const std::string& name) {
    throw std::runtime_error("isn't a section");
}