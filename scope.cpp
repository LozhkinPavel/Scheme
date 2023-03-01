#include "scope.h"
#include "functions.h"
#include "object.h"
#include "error.h"

Scope::Scope() {
    GlobalInitialization(*this);
    prev_ = nullptr;
}

Scope::Scope(Scope* rhs) {
    prev_ = rhs;
}

void Scope::Change(const std::string s, Name* k) {
    Scope* cur = this;
    while (cur != nullptr) {
        try {
            cur->scope_.at(s) = k;
            return;
        } catch (...) {
        }
        cur = cur->prev_;
    }
    throw NameError();
}

Name* Scope::Get(const std::string& s) {
    Scope* cur = this;
    while (cur != nullptr) {
        try {
            return cur->scope_.at(s);
        } catch (...) {
        }
        cur = cur->prev_;
    }
    throw NameError();
}

bool Scope::Find(const std::string s) {
    Scope* cur = this;
    while (cur != nullptr) {
        if (cur->scope_.find(s) != cur->scope_.end()) {
            return true;
        }
        cur = cur->prev_;
    }
    return false;
}

void Scope::ScopeMark() {
    if (mark_) {
        return;
    }
    mark_ = true;
    for (auto p : scope_) {
        if (auto ptr = dynamic_cast<Object*>(p.second)) {
            ptr->Mark();
        }
    }
    if (prev_ == nullptr) {
        return;
    }
    prev_->ScopeMark();
}