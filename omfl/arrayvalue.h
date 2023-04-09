#pragma once
#include "value.h"

namespace omfl {
    class ArrayValue : public Value {
    public:
        static Value* none_value;

        ArrayValue(omfl::Value::types type);

        ~ArrayValue();

        Value& operator[](std::size_t index) override;

        size_t size() const override;

        void AddValue(omfl::Value* value);

    private:
        std::vector<Value*> values_;
    };
}


