#pragma once

#include <filesystem>
#include <istream>
#include <cctype>
#include <iostream>
#include <map>
#include "tid.h"
#include "termvalue.h"
#include "arrayvalue.h"
#include <fstream>

namespace omfl {

    class Parser {
    public:
        Parser(const std::string& str);
        Parser(const std::filesystem::path& path);
        ~Parser();
        bool valid() const;
        void Parse();
        omfl::Object* Get(const std::string& key) const;
        omfl::Object* GetRecursive(Tid* ptr, const std::string& key) const;
    private:
        inline bool IsNameSym(char sym) const;
        inline bool IsValueSym(char sym) const;
        inline bool Empty(char sym) const;

        void GetChar(bool in_string = false);
        void GetFirstChar();

        void File();
        void KeyValue();
        void Section();
        void Comment();

        omfl::Value* Array();
        omfl::Value* Value();
        omfl::Value* Number();
        omfl::Value* Bool();
        omfl::Value* String();
        std::string Name();

        void AddKeyValue(const std::string& key, omfl::Value* value);

        char cur_char;
        Tid* global_tid;
        Tid* cur_tid;

        std::istream* stream_;
        bool correct_input;
    };

    Parser parse(const std::filesystem::path& path);

    Parser parse(const std::string& str);
}// namespace