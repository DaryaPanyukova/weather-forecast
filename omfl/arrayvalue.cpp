#include "arrayvalue.h"


omfl::Value* omfl::ArrayValue::none_value = new omfl::Value(omfl::Value::types::NONE);

omfl::ArrayValue::ArrayValue(omfl::Value::types type) : omfl::Value(type) {}

omfl::Value& omfl::ArrayValue::operator[](std::size_t index) {
    if (index > values_.size()) {
        return *none_value;
    }
    return *values_[index];
}

size_t omfl::ArrayValue::size() const {
    return values_.size();
}

void omfl::ArrayValue::AddValue(omfl::Value* value) {
    values_.push_back(value);
}

omfl::ArrayValue::~ArrayValue() {
    for (size_t i = 0; i < values_.size(); ++i) {
        delete values_[i];
    }
}


