#pragma once

#include <string>
#include "scope.h"

class Interpreter {
public:
    Interpreter();
    std::string Run(const std::string& s);
    ~Interpreter();

private:
    Scope* scope_;
};
