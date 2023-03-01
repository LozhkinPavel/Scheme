#pragma once

#include "object.h"
#include "scope.h"

class Name {
public:
    virtual ~Name() = default;
    virtual Object* Eval(Scope* cur_scope, Object* to_eval) = 0;
};

class Variable : public Name, public Object {
    friend class Heap;

public:
    Object* Eval(Scope* cur_scope, Object* to_eval);

private:
    Variable(Object* val) : val_(val) {
    }
    Object* val_;
};

class Function : public Name {
public:
    virtual ~Function() = default;
    virtual Object* Eval(Scope* cur_scope, Object* to_eval) = 0;
};

void GlobalInitialization(Scope& scope);

std::vector<Function*>& GetGlobalFunctions();

class MyLambda : public Function, public Object {
    friend class Heap;

public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
    void Mark() override;

private:
    MyLambda(Object* val, Scope* ptr) : val_(val), scope_(ptr) {
    }
    Object* val_;
    Scope* scope_ = nullptr;
};

class Plus : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Minus : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Prod : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Divide : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Greater : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Less : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class LEQ : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class GEQ : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Equal : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Max : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Min : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Abs : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class IsNumber : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class IsBoolean : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class IsSymbol : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Not : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class And : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Or : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class List : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class ListRef : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class ListTail : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class IsList : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};
class IsPair : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class IsNull : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Car : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Cdr : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Cons : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Quote : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class If : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Lambda : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Define : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class Set : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class SetCar : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};

class SetCdr : public Function {
public:
    Object* Eval(Scope* cur_scope, Object* to_eval) override;
};
