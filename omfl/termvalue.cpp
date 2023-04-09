#include "termvalue.h"


omfl::TermValue::TermValue(omfl::Value::types type) : omfl::Value(type) {}


int32_t omfl::TermValue::AsInt() const {
    return std::stoi(value_);
}

int32_t omfl::TermValue::AsIntOrDefault(int32_t default_value) const {
    if (IsInt()) {
        return AsInt();
    } else {
        return default_value;
    }
}

double omfl::TermValue::AsFloat() const {
    return ::atof(value_.c_str());
}

double omfl::TermValue::AsFloatOrDefault(double default_value) const {
    if (IsFloat()) {
        return AsFloat();
    } else {
        return default_value;
    }
}

std::string omfl::TermValue::AsString() const {
    return value_;
}

std::string
omfl::TermValue::AsStringOrDefault(const std::string& default_value) const {
    if (IsString()) {
        return AsString();
    } else {
        return default_value;
    }
}
bool omfl::TermValue::AsBool() const {
    return (value_ == "true");
}

bool omfl::TermValue::AsBoolOrDefault(bool default_value) const {
    if (IsBool()) {
        return AsBool();
    } else {
        return default_value;
    }
}

void omfl::TermValue::AddChar(char sym) {
    value_ += sym;
}

const std::string& omfl::TermValue::GetValue() const {
    return value_;
}
