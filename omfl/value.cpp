#include "value.h"

omfl::Value::Value(omfl::Value::types type0) {
    type_ = type0;
}

bool omfl::Value::IsInt() const {
    return type_ == INT;
}

bool omfl::Value::IsFloat() const {
    return type_ == FLOAT;
}

bool omfl::Value::IsString() const {
    return type_ == STRING;
}

bool omfl::Value::IsBool() const {
    return type_ == BOOL;
}

bool omfl::Value::IsArray() const {
    return type_ == ARRAY;
}


int32_t omfl::Value::AsInt() const {
    throw std::runtime_error("isn't a terminal value");
}

int32_t omfl::Value::AsIntOrDefault(int32_t default_value) const {
    return default_value;
}

double omfl::Value::AsFloat() const {
    throw std::runtime_error("isn't a value");
}

double omfl::Value::AsFloatOrDefault(double default_value) const {
    return default_value;
}

std::string omfl::Value::AsString() const {
    throw std::runtime_error("isn't a value");
}

std::string
omfl::Value::AsStringOrDefault(const std::string& default_value) const {
    return default_value;
}

bool omfl::Value::AsBool() const {
    throw std::runtime_error("isn't a value");
}

bool omfl::Value::AsBoolOrDefault(bool default_value) const {
    return default_value;
}


omfl::Value& omfl::Value::operator[](std::size_t index) {
    throw std::runtime_error("isn't an array");
}

size_t omfl::Value::size() const {
    throw std::runtime_error("isn't an array");
}

void omfl::Value::AddChar(char sym) {
    throw std::runtime_error("isn't an value");
}

omfl::Value::types omfl::Value::GetType() const {
    return type_;
}

void omfl::Value::SetType(omfl::Value::types type) {
    type_ = type;
}

const std::string& omfl::Value::GetValue() const {
    throw std::runtime_error("isn't an value");
}

void omfl::Value::AddValue(omfl::Value* value) {
    throw std::runtime_error("isn't an array");
}

