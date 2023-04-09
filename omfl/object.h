#pragma once
#include <iostream>
#include <exception>

namespace omfl {
    class Object {
    public:
        virtual ~Object() = default;

        virtual bool IsInt() const;

        virtual int32_t AsInt() const;

        virtual int32_t AsIntOrDefault(int32_t default_value) const;

        virtual bool IsFloat() const;

        virtual double AsFloat() const;

        virtual double AsFloatOrDefault(double default_value) const;

        virtual bool IsString() const;

        virtual std::string AsString() const;

        virtual std::string
        AsStringOrDefault(const std::string& default_value) const;

        virtual bool IsBool() const;

        virtual bool AsBool() const;

        virtual bool AsBoolOrDefault(bool default_value) const;

        virtual bool IsArray() const;

        virtual Object& operator[](std::size_t index);

        virtual size_t size() const;

        virtual Object& Get(const std::string& name);
    };
}


