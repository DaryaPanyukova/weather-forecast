#pragma once

#include <iostream>
#include <vector>
#include "object.h"

namespace omfl {
    class Value : public Object {
    public:
        enum types {
            INT,
            FLOAT,
            BOOL,
            STRING,
            ARRAY,
            NONE
        };

        explicit Value(types type0);

        ~Value() override = default;

        bool IsInt() const override;

        bool IsFloat() const override;

        bool IsString() const override;

        bool IsBool() const override;

        bool IsArray() const override;

        int32_t AsInt() const override;

        int32_t AsIntOrDefault(int32_t default_value) const override;

        double AsFloat() const override;

        double AsFloatOrDefault(double default_value) const override;

        std::string AsString() const override;

        std::string
        AsStringOrDefault(const std::string& default_value) const override;

        bool AsBool() const override;

        bool AsBoolOrDefault(bool default_value) const override;

        virtual void AddChar(char sym);

        virtual const std::string& GetValue() const;

        Value& operator[](std::size_t index) override;

        size_t size() const override;

        virtual void AddValue(Value* value);

        types GetType() const;

        void SetType(types type);

    private:
        types type_;
    };
}