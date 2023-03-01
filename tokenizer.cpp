#include <tokenizer.h>
#include "error.h"

bool SymbolToken::operator==(const SymbolToken &other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken &) const {
    return true;
}

bool DotToken::operator==(const DotToken &) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken &other) const {
    return value == other.value;
}

bool BooleanToken::operator==(const BooleanToken &other) const {
    return flag == other.flag;
}

Tokenizer::Tokenizer(std::istream *in) : stream_(in) {
    Next();
}

Tokenizer::~Tokenizer() {
    if (nxt_char_) {
        delete nxt_char_;
    }
}

bool Tokenizer::IsOpenSymbol(int a) {
    bool flag1 = (a <= 'z') & (a >= 'a');
    bool flag2 = (a <= 'Z') & (a >= 'A');
    bool flag3 = false;
    std::string s = "<=>*//#";
    for (int x : s) {
        if (a == x) {
            flag3 = true;
        }
    }
    return flag1 | flag2 | flag3;
}

bool Tokenizer::IsSymbol(int a) {
    if (IsOpenSymbol(a)) {
        return true;
    }
    bool flag = (a <= '9') & (a >= '0');
    return (a == '?') | (a == '!') | (a == '-') | flag;
}

void Tokenizer::GetSymbolToken(int cur) {
    std::string token;
    token.push_back(cur);
    int cur_char = stream_->get();
    while (IsSymbol(cur_char)) {
        token.push_back(cur_char);
        cur_char = stream_->get();
    }
    cur_token_ = Token{SymbolToken{token}};
    nxt_char_ = new int(cur_char);
}

void Tokenizer::GetNumberToken(bool sign, int cur) {
    int number = 0;
    while (cur >= '0' && cur <= '9') {
        number *= 10;
        number += cur - '0';
        cur = stream_->get();
    }
    if (!sign) {
        number *= -1;
    }
    nxt_char_ = new int(cur);
    cur_token_ = Token{ConstantToken{number}};
}

void Tokenizer::Next() {
    if (is_end_) {
        return;
    }
    int cur_char;
    if (nxt_char_) {
        cur_char = *nxt_char_;
        delete nxt_char_;
        nxt_char_ = nullptr;
    } else {
        cur_char = stream_->get();
    }
    while (cur_char == ' ' || cur_char == '\n') {
        cur_char = stream_->get();
    }
    if (cur_char == EOF) {
        is_end_ = true;
        return;
    }
    if (cur_char == '#') {
        cur_char = stream_->get();
        if (cur_char == 'f') {
            cur_token_ = Token{BooleanToken{false}};
            return;
        }
        if (cur_char == 't') {
            cur_token_ = Token{BooleanToken{true}};
            return;
        }
        GetSymbolToken(cur_char);
        return;
    }
    if (cur_char == '\'') {
        cur_token_ = Token{QuoteToken{}};
        return;
    }
    if (cur_char == '(') {
        cur_token_ = Token{BracketToken::OPEN};
        return;
    }
    if (cur_char == ')') {
        cur_token_ = Token{BracketToken::CLOSE};
        return;
    }
    if (cur_char == '.') {
        cur_token_ = Token{DotToken{}};
        return;
    }
    if (cur_char == '+') {
        cur_char = stream_->get();
        if (cur_char < '0' || cur_char > '9') {
            nxt_char_ = new int(cur_char);
            cur_token_ = Token{SymbolToken{"+"}};
        } else {
            GetNumberToken(true, cur_char);
        }
        return;
    }
    if (cur_char == '-') {
        cur_char = stream_->get();
        if (cur_char < '0' || cur_char > '9') {
            nxt_char_ = new int(cur_char);
            cur_token_ = Token{SymbolToken{"-"}};
        } else {
            GetNumberToken(false, cur_char);
        }
        return;
    }
    if (cur_char >= '0' && cur_char <= '9') {
        GetNumberToken(true, cur_char);
        return;
    }
    if (IsOpenSymbol(cur_char)) {
        GetSymbolToken(cur_char);
        return;
    }
    throw SyntaxError();
}

Token Tokenizer::GetToken() {
    return cur_token_;
}

bool Tokenizer::IsEnd() {
    return is_end_;
}