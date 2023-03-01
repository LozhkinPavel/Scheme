#include "scheme.h"
#include "heap.h"
#include "error.h"
#include "parser.h"
#include <sstream>

std::string ToStr(Object* x, bool flag) {
    if (x == nullptr) {
        return "()";
    }
    if (auto ptr = As<Symbol>(x)) {
        return ptr->GetName();
    }
    if (auto ptr = As<Number>(x)) {
        return std::to_string(ptr->GetValue());
    }
    if (auto ptr = As<Boolean>(x)) {
        if (ptr->GetFlag()) {
            return "#t";
        } else {
            return "#f";
        }
    }
    std::string ans;
    if (flag) {
        ans += "(";
    }
    ans += ToStr(As<Cell>(x)->GetFirst(), true);
    if (As<Cell>(x)->GetSecond() != nullptr) {
        ans += " ";
    }
    if (!Is<Cell>(As<Cell>(x)->GetSecond()) && As<Cell>(x)->GetSecond() != nullptr) {
        ans += ". ";
    }
    if (As<Cell>(x)->GetSecond() != nullptr) {
        ans += ToStr(As<Cell>(x)->GetSecond(), false);
    }
    if (flag) {
        ans += ")";
    }
    return ans;
}

Object* Evaluate(Scope* cur_scope, Object* exp) {
    if (exp == nullptr) {
        throw RuntimeError();
    }
    if (auto ptr = As<Cell>(exp)) {
        if (!Is<Symbol>(ptr->GetFirst())) {
            if (ptr->GetFirst() == nullptr) {
                throw RuntimeError();
            }
            auto val = Evaluate(cur_scope, ptr->GetFirst());
            if (!Is<MyLambda>(val)) {
                throw RuntimeError();
            }
            return As<MyLambda>(val)->Eval(cur_scope, ptr->GetSecond());
        }
        auto x = (*cur_scope).Get(As<Symbol>(ptr->GetFirst())->GetName());
        Object* ans = x->Eval(cur_scope, ptr->GetSecond());
        return ans;
    } else {
        if (Is<Symbol>(exp)) {
            auto x = (*cur_scope).Get(As<Symbol>(exp)->GetName());
            if (auto y = dynamic_cast<MyLambda*>(x)) {
                return y;
            }
            if (dynamic_cast<Variable*>(x)) {
                return x->Eval(cur_scope, nullptr);
            }
            throw RuntimeError();
        }
        return exp;
    }
}

Interpreter::Interpreter() : scope_(GetHeap().Make()) {
}

Interpreter::~Interpreter() {
    for (auto ptr : GetGlobalFunctions()) {
        delete ptr;
    }
    GetGlobalFunctions().clear();
    GetHeap().Clear();
}

std::string Interpreter::Run(const std::string& s) {
    std::stringstream ss;
    ss << s;
    Tokenizer tokenizer(&ss);
    Object* exp = Read(&tokenizer);
    if (!tokenizer.IsEnd() || Is<CloseBracket>(exp) || Is<Dot>(exp)) {
        throw SyntaxError();
    }
    auto ans = ToStr(Evaluate(scope_, exp), true);
    scope_->ScopeMark();
    GetHeap().Delete();
    return ans;
}
