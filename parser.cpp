#include <parser.h>
#include <error.h>
#include <heap.h>

Object* Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError();
    }
    Token cur = tokenizer->GetToken();
    tokenizer->Next();
    if (auto ptr = std::get_if<BracketToken>(&cur)) {
        if (*ptr == BracketToken::OPEN) {
            return ReadList(tokenizer);
        } else {
            return GetHeap().Make<CloseBracket>();
        }
    }
    if (auto ptr = std::get_if<SymbolToken>(&cur)) {
        return GetHeap().Make<Symbol>(ptr->name);
    }
    if (auto ptr = std::get_if<ConstantToken>(&cur)) {
        return GetHeap().Make<Number>(ptr->value);
    }
    if (auto ptr = std::get_if<BooleanToken>(&cur)) {
        return GetHeap().Make<Boolean>(ptr->flag);
    }
    if (std::get_if<DotToken>(&cur)) {
        return GetHeap().Make<Dot>();
    }
    if (std::get_if<QuoteToken>(&cur)) {
        auto x = Read(tokenizer);
        if (Is<Dot>(x) || Is<CloseBracket>(x)) {
            throw SyntaxError();
        }
        auto a = GetHeap().Make(x, static_cast<Object*>(nullptr));
        return GetHeap().Make(GetHeap().Make<Symbol>("quote"), a);
    }
    throw SyntaxError();
}

Object* ReadList(Tokenizer* tokenizer) {
    Object* ans = nullptr;
    Object* tail = ans;
    Object* cur;
    bool flag = false;
    bool placed = false;
    while (!Is<CloseBracket>(cur = Read(tokenizer))) {
        if (placed) {
            throw SyntaxError();
        }
        if (!ans) {
            if (Is<CloseBracket>(cur) || Is<Dot>(cur)) {
                throw SyntaxError();
            }
            ans = GetHeap().Make(cur, static_cast<Object*>(nullptr));
            tail = ans;
        } else {
            if (flag) {
                placed = true;
                if (Is<CloseBracket>(cur) || Is<Dot>(cur)) {
                    throw SyntaxError();
                }
                As<Cell>(tail)->ReplaceSecond(cur);
            } else {
                if (Is<Dot>(cur)) {
                    flag = true;
                } else {
                    if (Is<CloseBracket>(cur) || Is<Dot>(cur)) {
                        throw SyntaxError();
                    }
                    As<Cell>(tail)->ReplaceSecond(
                        GetHeap().Make(cur, static_cast<Object*>(nullptr)));
                    tail = As<Cell>(tail)->GetSecond();
                }
            }
        }
    }
    if (flag && !placed) {
        throw SyntaxError();
    }
    return ans;
}