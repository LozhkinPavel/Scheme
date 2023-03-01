#pragma once

#include <variant>
#include <optional>
#include <istream>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const;
};

struct BooleanToken {
    bool flag;

    bool operator==(const BooleanToken& other) const;
};

using Token =
    std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, BooleanToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);
    ~Tokenizer();

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    void GetNumberToken(bool sign, int cur);
    void GetSymbolToken(int cur);
    bool IsSymbol(int a);
    bool IsOpenSymbol(int a);
    std::istream* stream_;
    Token cur_token_;
    int* nxt_char_ = nullptr;
    bool is_end_ = false;
};