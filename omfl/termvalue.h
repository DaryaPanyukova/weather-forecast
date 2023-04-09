#pragma once
#include "value.h"

namespace omfl {
    class TermValue : public Value {
    public:
        TermValue(omfl::Value::types type);

        int32_t AsInt() const override;

        int32_t AsIntOrDefault(int32_t default_value) const override;

        double AsFloat() const override;

        double AsFloatOrDefault(double default_value) const override;

        std::string AsString() const override;

        std::string AsStringOrDefault(const std::string& default_value) const override;

        bool AsBool() const override;

        bool AsBoolOrDefault(bool default_value) const override;

        void AddChar(char sym) override;

        const std::string& GetValue() const override;
    private:
        std::string value_; // bool/string/int/float
    };
}

