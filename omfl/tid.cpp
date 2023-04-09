#include "tid.h"

omfl::Object& omfl::Tid::Get(const std::string& name) {
    if (subsections_.find(name) != subsections_.end()) {
        return *subsections_[name];
    } else {
        return *key_value_[name];
    }
}

bool omfl::Tid::HasSubsection(const std::string& name) const {
    return subsections_.find(name) != subsections_.end();
}

bool omfl::Tid::HasKey(const std::string& key) const {
    return key_value_.find(key) != key_value_.end();
}

void omfl::Tid::AddSubsection(const std::string& name) {
    subsections_[name] = new Tid;
}

omfl::Tid* omfl::Tid::GetSubsection(const std::string& name) {
    return subsections_[name];
}

void omfl::Tid::SetValue(const std::string& key, omfl::Value* value) {
    key_value_[key] = value;
}

omfl::Value* omfl::Tid::GetValue(const std::string& key) {
    return key_value_[key];
}

omfl::Tid::~Tid() {
    for (const auto& elem: subsections_) {
        delete elem.second;
    }

    for (const auto& elem: key_value_) {
        delete elem.second;
    }
}
