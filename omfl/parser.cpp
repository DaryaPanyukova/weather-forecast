#include "parser.h"

void omfl::Parser::Parse() {
    File();
}

omfl::Parser omfl::parse(const std::string& str) {
    omfl::Parser parser(str);
    parser.Parse();
    return parser;
}


omfl::Parser::Parser(const std::string& str) {
    stream_ = new std::stringstream(str);
    global_tid = new Tid;
    cur_tid = global_tid;
    GetFirstChar();
    correct_input = true;
}

omfl::Parser::Parser(const std::filesystem::path& path) {
    stream_ = new std::ifstream(path);
    global_tid = new Tid;
    cur_tid = global_tid;
    GetFirstChar();
    correct_input = true;
}


inline bool omfl::Parser::IsNameSym(char sym) const {
    return isdigit(sym) || isalpha(sym) || (sym == '-') || (sym == '_');
}

inline bool omfl::Parser::IsValueSym(char sym) const {
    return std::isdigit(sym) || sym == '+' ||
           sym == '-' || sym == '\"' || sym == '[' || sym == 't' || sym == 'f';
}

inline bool omfl::Parser::Empty(char sym) const {
    return (sym == '\0');
}

void omfl::Parser::GetFirstChar() {
    char tmp;
    stream_->read(&tmp, 1);
    while (tmp == ' ' || tmp == '\n') {
        stream_->read(&tmp, 1);
    }
    cur_char = tmp;
}

void omfl::Parser::GetChar(bool in_string) {
    char tmp;
    if (!stream_->read(&tmp, 1)) {
        cur_char = '\0';
        return;
    }

    if (!in_string) {
        while (tmp == ' ') {
            if (!stream_->read(&tmp, 1)) {
                cur_char = '\0';
                return;
            }
        }
    }

    cur_char = tmp;
}

bool omfl::Parser::valid() const {
    return correct_input;
}

void omfl::Parser::File() {
    while (cur_char != '\0') {
        if (cur_char == '[') {
            Section();
        } else if (IsNameSym(cur_char)) {
            KeyValue();
        } else if (cur_char == '#') {
            Comment();
        } else if (cur_char == '\n') {
            GetChar();
        } else {
            correct_input = false;
            return;
        }
    }
}

void omfl::Parser::KeyValue() {
    std::string key_name = Name();

    if (Empty(cur_char) || cur_char != '=') {
        correct_input = false;
    }
    GetChar();
    if (IsValueSym(cur_char)) {
        omfl::Value* value = Value();
        AddKeyValue(key_name, value);
    } else {
        correct_input = false;
    }
}

void omfl::Parser::Section() {
    GetChar();
    cur_tid = global_tid;
    while (!Empty(cur_char)) {
        if (IsNameSym(cur_char)) {
            std::string name = Name();
            if (!cur_tid->HasSubsection(name)) {
                cur_tid->AddSubsection(name);
            }
            cur_tid = cur_tid->GetSubsection(name);
            if (cur_char != '.') {
                break;
            } else {
                GetChar();
                continue;
            }
        } else {
            correct_input = false;
            break;
        }
    }
    if (cur_char != ']') {
        correct_input = false;
    }
    GetChar();
}

void omfl::Parser::Comment() {
    while (!Empty(cur_char) && cur_char != '\n') {
        GetChar(true);
    }
}

omfl::Value* omfl::Parser::Array() {
    omfl::ArrayValue* val = new omfl::ArrayValue(Value::ARRAY);
    GetChar();

    while (!Empty(cur_char) && IsValueSym(cur_char)) {
        val->AddValue(Value());
        if (cur_char != ',') {
            break;
        } else {
            GetChar();
            continue;
        }

    }
    if (cur_char != ']') {
        correct_input = false;
    }
    GetChar();
    return val;
}

omfl::Value* omfl::Parser::Value() {
    if (std::isdigit(cur_char) || cur_char == '+' || cur_char == '-') {
        return Number();
    } else if (cur_char == '\"') {
        return String();
    } else if (cur_char == '[') {
        return Array();
    } else if (cur_char == 't' || cur_char == 'f') {
        return Bool();
    } else {
        correct_input = false; // actually never gonna happen
        return nullptr;
    }
}

omfl::Value* omfl::Parser::Number() {
    omfl::TermValue* val = new omfl::TermValue(Value::INT);
    if (cur_char == '+' || cur_char == '-') {
        val->AddChar(cur_char);
        GetChar();
    }
    if (!std::isdigit(cur_char)) {
        correct_input = false;
    }
    while (std::isdigit(cur_char)) {
        val->AddChar(cur_char);
        GetChar();
    }
    if (cur_char == '.') {
        val->SetType(Value::FLOAT);
        val->AddChar(cur_char);
        GetChar();
        if (!std::isdigit(cur_char)) {
            correct_input = false;
        }
        while (std::isdigit(cur_char)) {
            val->AddChar(cur_char);
            GetChar();
        }
    }
    return val;
}

omfl::Value* omfl::Parser::Bool() {
    omfl::TermValue* val = new omfl::TermValue(Value::BOOL);
    while (!Empty(cur_char) && std::isalpha(cur_char)) {
        val->AddChar(cur_char);
        GetChar(true);
    }
    if (val->GetValue() == "true" || val->GetValue() == "false") {
        return val;
    } else {
        correct_input = false;
        return nullptr;
    }

}

omfl::Value* omfl::Parser::String() {
    omfl::TermValue* val = new omfl::TermValue(Value::STRING);

    GetChar(true);
    while (!Empty(cur_char)) {
        if (cur_char == '\"') {
            break;
        }
        val->AddChar(cur_char);
        GetChar(true);
    }
    if (Empty(cur_char)) {
        correct_input = false;
    }
    GetChar();
    return val;
}

std::string omfl::Parser::Name() {
    std::string name = "";
    while (!Empty(cur_char)) {
        if (!IsNameSym(cur_char)) {
            break;
        }
        name += cur_char;
        GetChar();
    }
    if (name.empty()) {
        correct_input = false;
    }
    return name;
}

omfl::Object*
omfl::Parser::GetRecursive(omfl::Tid* ptr, const std::string& key) const {
    if (ptr->HasKey(key)) {
        return ptr->GetValue(key);
    }

    size_t ind = key.find('.');
    std::string section = key.substr(0, ind);
    std::string subsection = key.substr(ind + 1, key.size());
    return GetRecursive(ptr->GetSubsection(section), subsection);
}


omfl::Object* omfl::Parser::Get(const std::string& key) const {
    if (global_tid->HasSubsection(key)) {
        return global_tid->GetSubsection(key);
    }
    return GetRecursive(global_tid, key);
}

void omfl::Parser::AddKeyValue(const std::string& key, omfl::Value* value) {
    if (cur_tid->HasKey(key)) {
        correct_input = false;
        return;
    }
    cur_tid->SetValue(key, value);
}

omfl::Parser::~Parser() {
    delete global_tid;
    delete stream_;
}



