#pragma once
#include <iostream>
#include <unordered_map>
#include "value.h"

namespace omfl {
    class Tid : public Object {
    public:
        ~Tid() override;
        Object& Get(const std::string& name) override;
        bool HasSubsection(const std::string& name) const;
        bool HasKey(const std::string& key) const;
        void AddSubsection(const std::string& name);
        Tid* GetSubsection(const std::string& name);
        void SetValue(const std::string& key, omfl::Value* value);
        omfl::Value* GetValue(const std::string& key);
    private:
        std::unordered_map<std::string, Value*> key_value_;
        std::unordered_map<std::string, Tid*> subsections_;
    };
}
