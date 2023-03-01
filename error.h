#pragma once

#include <stdexcept>

struct SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    SyntaxError() : std::runtime_error("") {
    }
};

struct RuntimeError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    RuntimeError() : std::runtime_error("") {
    }
};

struct NameError : public std::runtime_error {
    using std::runtime_error::runtime_error;
    NameError() : std::runtime_error("") {
    }
};
