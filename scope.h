#pragma once

#include <unordered_map>
#include <string>

class Name;

class Scope {
    friend class Heap;

public:
    void Register(const std::string s, Name* k) {
        scope_[s] = k;
    }
    bool Find(const std::string s);
    void Change(const std::string s, Name* k);
    Name* Get(const std::string& s);
    void ScopeMark();

private:
    Scope();
    Scope(Scope* rhs);
    std::unordered_map<std::string, Name*> scope_;
    Scope* prev_ = nullptr;
    bool mark_ = false;
};
