#include "functions.h"
#include "heap.h"
#include "error.h"

Object* Evaluate(Scope* cur_scope, Object* exp);

void ToVector(Object* list, std::vector<Object*>& ans) {
    auto cur = list;
    if (cur == nullptr) {
        return;
    }
    if (Is<Cell>(cur)) {
        ans.push_back(As<Cell>(cur)->GetFirst());
        ToVector(As<Cell>(cur)->GetSecond(), ans);
        return;
    }
    ans.push_back(cur);
}

std::vector<Function*>& GetGlobalFunctions() {
    static std::vector<Function*> global_functions;
    return global_functions;
}

void GlobalInitialization(Scope& scope) {
    Function* ptr = new Plus();
    scope.Register("+", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Minus();
    scope.Register("-", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Divide();
    scope.Register("/", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Prod();
    scope.Register("*", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Max();
    scope.Register("max", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Min();
    scope.Register("min", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Abs();
    scope.Register("abs", ptr);
    GetGlobalFunctions().push_back(ptr);

    ptr = new Greater();
    scope.Register(">", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Less();
    scope.Register("<", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new LEQ();
    scope.Register("<=", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new GEQ();
    scope.Register(">=", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Equal();
    scope.Register("=", ptr);
    GetGlobalFunctions().push_back(ptr);

    ptr = new IsNumber();
    scope.Register("number?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new IsBoolean();
    scope.Register("boolean?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new IsSymbol();
    scope.Register("symbol?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Not();
    scope.Register("not", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Or();
    scope.Register("or", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new And();
    scope.Register("and", ptr);
    GetGlobalFunctions().push_back(ptr);

    ptr = new List();
    scope.Register("list", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new ListRef();
    scope.Register("list-ref", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new ListTail();
    scope.Register("list-tail", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Car();
    scope.Register("car", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Cdr();
    scope.Register("cdr", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Cons();
    scope.Register("cons", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new IsList();
    scope.Register("list?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new IsPair();
    scope.Register("pair?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new IsNull();
    scope.Register("null?", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Quote();
    scope.Register("quote", ptr);
    GetGlobalFunctions().push_back(ptr);

    ptr = new If();
    scope.Register("if", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Define();
    scope.Register("define", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Lambda();
    scope.Register("lambda", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new SetCar();
    scope.Register("set-car!", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new SetCdr();
    scope.Register("set-cdr!", ptr);
    GetGlobalFunctions().push_back(ptr);
    ptr = new Set();
    scope.Register("set!", ptr);
    GetGlobalFunctions().push_back(ptr);
}

template <typename Func>
Object* PlusProd(Scope* cur_scope, Object* to_eval, Func func, int64_t init) {
    int64_t ans = init;
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    for (auto ptr : vec) {
        if (ptr == nullptr) {
            throw RuntimeError();
        }
        auto val = Evaluate(cur_scope, ptr);
        if (!Is<Number>(val)) {
            throw RuntimeError();
        }
        ans = func(ans, As<Number>(val)->GetValue());
    }
    return GetHeap().Make<Number>(ans);
}

template <typename Func>
Object* MinusDiv(Scope* cur_scope, Object* to_eval, Func func) {
    int64_t ans = 0;
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (to_eval == nullptr) {
        throw RuntimeError();
    }
    auto init = Evaluate(cur_scope, vec[0]);
    if (!Is<Number>(init)) {
        throw RuntimeError();
    }
    ans = As<Number>(init)->GetValue();

    for (size_t i = 1; i < vec.size(); ++i) {
        auto ptr = vec[i];
        if (ptr == nullptr) {
            throw RuntimeError();
        }
        auto val = Evaluate(cur_scope, ptr);
        if (!Is<Number>(val)) {
            throw RuntimeError();
        }
        ans = func(ans, As<Number>(val)->GetValue());
    }
    return GetHeap().Make<Number>(ans);
}

template <typename Func>
Object* BooleanFunc(Scope* cur_scope, Object* to_eval, Func func) {
    bool ans = true;
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    for (size_t i = 1; i < vec.size(); ++i) {
        auto ptr1 = vec[i - 1];
        auto ptr2 = vec[i];
        if (ptr2 == nullptr) {
            continue;
        }
        auto val1 = Evaluate(cur_scope, ptr1);
        auto val2 = Evaluate(cur_scope, ptr2);
        if (!Is<Number>(val1) || !Is<Number>(val2)) {
            throw RuntimeError();
        }
        ans &= func(As<Number>(val1)->GetValue(), As<Number>(val2)->GetValue());
    }
    return GetHeap().Make<Boolean>(ans);
}

Object* Plus::Eval(Scope* cur_scope, Object* to_eval) {
    return PlusProd(
        cur_scope, to_eval, [](int64_t a, int64_t b) { return a + b; }, 0);
}

Object* Minus::Eval(Scope* cur_scope, Object* to_eval) {
    return MinusDiv(cur_scope, to_eval, [](int64_t a, int64_t b) { return a - b; });
}

Object* Prod::Eval(Scope* cur_scope, Object* to_eval) {
    return PlusProd(
        cur_scope, to_eval, [](int64_t a, int64_t b) { return a * b; }, 1);
}

Object* Divide::Eval(Scope* cur_scope, Object* to_eval) {
    return MinusDiv(cur_scope, to_eval, [](int64_t a, int64_t b) { return a / b; });
}

Object* Greater::Eval(Scope* cur_scope, Object* to_eval) {
    return BooleanFunc(cur_scope, to_eval, [](int64_t a, int64_t b) { return a > b; });
}

Object* Less::Eval(Scope* cur_scope, Object* to_eval) {
    return BooleanFunc(cur_scope, to_eval, [](int64_t a, int64_t b) { return a < b; });
}

Object* LEQ::Eval(Scope* cur_scope, Object* to_eval) {
    return BooleanFunc(cur_scope, to_eval, [](int64_t a, int64_t b) { return a <= b; });
}
Object* GEQ::Eval(Scope* cur_scope, Object* to_eval) {
    return BooleanFunc(cur_scope, to_eval, [](int64_t a, int64_t b) { return a >= b; });
}
Object* Equal::Eval(Scope* cur_scope, Object* to_eval) {
    return BooleanFunc(cur_scope, to_eval, [](int64_t a, int64_t b) { return a == b; });
}

Object* Max::Eval(Scope* cur_scope, Object* to_eval) {
    return MinusDiv(cur_scope, to_eval, [](int64_t a, int64_t b) { return std::max(a, b); });
}

Object* Min::Eval(Scope* cur_scope, Object* to_eval) {
    return MinusDiv(cur_scope, to_eval, [](int64_t a, int64_t b) { return std::min(a, b); });
}

Object* Abs::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (to_eval == nullptr) {
        throw RuntimeError();
    }
    if (!Is<Number>(vec[0]) || vec.size() > 1) {
        throw RuntimeError();
    }
    int64_t ans = As<Number>(Evaluate(cur_scope, vec[0]))->GetValue();
    return GetHeap().Make<Number>(std::abs(ans));
}

Object* IsNumber::Eval(Scope* cur_scope, Object* to_eval) {
    if (to_eval == nullptr) {
        throw RuntimeError();
    }
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() > 1) {
        throw RuntimeError();
    }
    return GetHeap().Make<Boolean>(Is<Number>(Evaluate(cur_scope, vec[0])));
}

Object* IsBoolean::Eval(Scope* cur_scope, Object* to_eval) {
    if (to_eval == nullptr) {
        throw RuntimeError();
    }
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() > 1) {
        throw RuntimeError();
    }
    return GetHeap().Make<Boolean>(Is<Boolean>(Evaluate(cur_scope, vec[0])));
}

Object* IsSymbol::Eval(Scope* cur_scope, Object* to_eval) {
    if (to_eval == nullptr) {
        throw RuntimeError();
    }
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() > 1) {
        throw RuntimeError();
    }
    return GetHeap().Make<Boolean>(Is<Symbol>(Evaluate(cur_scope, vec[0])));
}

Object* Not::Eval(Scope* cur_scope, Object* to_eval) {
    bool ans = true;
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    if (auto ptr = As<Boolean>(val)) {
        ans = ptr->GetFlag();
    }
    return GetHeap().Make<Boolean>(!ans);
}

Object* And::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    for (auto ptr : vec) {
        auto val = Evaluate(cur_scope, ptr);
        bool cur = true;
        if (Is<Boolean>(val)) {
            cur = As<Boolean>(val)->GetFlag();
        }
        if (!cur) {
            return GetHeap().Make<Boolean>(false);
        }
    }
    if (to_eval == nullptr) {
        return GetHeap().Make<Boolean>(true);
    }
    return Evaluate(cur_scope, vec[vec.size() - 1]);
}

Object* Or::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    for (auto ptr : vec) {
        auto val = Evaluate(cur_scope, ptr);
        bool cur = true;
        if (Is<Boolean>(val)) {
            cur = As<Boolean>(val)->GetFlag();
        }
        if (cur) {
            return val;
        }
    }
    if (to_eval == nullptr) {
        return GetHeap().Make<Boolean>(false);
    }
    return Evaluate(cur_scope, vec[vec.size() - 1]);
}

Object* List::Eval(Scope* cur_scope, Object* to_eval) {
    return to_eval;
}

Object* ListRef::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 2) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[1]);
    if (!Is<Number>(val)) {
        throw RuntimeError();
    }
    size_t ind = As<Number>(val)->GetValue();
    auto ptr = Evaluate(cur_scope, vec[0]);
    while (ind > 0) {
        if (!Is<Cell>(ptr)) {
            throw RuntimeError();
        }
        ptr = As<Cell>(ptr)->GetSecond();
        ind--;
    }
    if (ptr == nullptr) {
        throw RuntimeError();
    }
    if (Is<Cell>(ptr)) {
        return As<Cell>(ptr)->GetFirst();
    }
    return ptr;
}

Object* ListTail::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 2) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[1]);
    if (!Is<Number>(val)) {
        throw RuntimeError();
    }
    size_t ind = As<Number>(val)->GetValue();
    auto ptr = Evaluate(cur_scope, vec[0]);
    while (ind > 0) {
        if (!Is<Cell>(ptr)) {
            throw RuntimeError();
        }
        ptr = As<Cell>(ptr)->GetSecond();
        ind--;
    }
    if (Is<Cell>(ptr) || ptr == nullptr) {
        return As<Cell>(ptr);
    }
    return GetHeap().Make(ptr, static_cast<Object*>(nullptr));
}

Object* IsList::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    while (auto ptr = As<Cell>(val)) {
        val = ptr->GetSecond();
    }
    return GetHeap().Make<Boolean>(val == nullptr);
}

Object* IsNull::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    return GetHeap().Make<Boolean>(val == nullptr);
}

Object* IsPair::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    return GetHeap().Make<Boolean>(Is<Cell>(val));
}

Object* Car::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    if (!Is<Cell>(val)) {
        throw RuntimeError();
    }
    return As<Cell>(val)->GetFirst();
}

Object* Cdr::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 1) {
        throw RuntimeError();
    }
    auto val = Evaluate(cur_scope, vec[0]);
    if (!Is<Cell>(val)) {
        throw RuntimeError();
    }
    return As<Cell>(val)->GetSecond();
}

Object* Cons::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 2) {
        throw RuntimeError();
    }
    auto val1 = Evaluate(cur_scope, vec[0]);
    auto val2 = Evaluate(cur_scope, vec[1]);
    return GetHeap().Make(val1, val2);
}

Object* Quote::Eval(Scope* cur_scope, Object* to_eval) {
    if (!Is<Cell>(to_eval)) {
        throw RuntimeError();
    }
    return As<Cell>(to_eval)->GetFirst();
}

Object* If::Eval(Scope* cur_scope, Object* to_eval) {
    Scope* new_scope(GetHeap().Make(cur_scope));
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 2 && vec.size() != 3) {
        throw SyntaxError();
    }
    auto val1 = Evaluate(new_scope, vec[0]);
    auto flag = true;
    if (auto ptr = As<Boolean>(val1)) {
        flag = ptr->GetFlag();
    }
    if (flag) {
        return Evaluate(new_scope, vec[1]);
    }
    if (vec.size() == 2) {
        return nullptr;
    }
    return Evaluate(new_scope, vec[2]);
}

Object* Define::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() <= 1) {
        throw SyntaxError();
    }
    if (auto ptr = As<Symbol>(vec[0])) {
        if (vec.size() != 2) {
            throw SyntaxError();
        }
        std::vector<Object*> body;
        ToVector(vec[1], body);
        if (Is<Symbol>(body[0]) && As<Symbol>(body[0])->GetName() == "lambda") {
            auto signature = As<Cell>(As<Cell>(vec[1])->GetSecond());
            Object* x(GetHeap().Make(signature->GetFirst(), signature->GetSecond()));
            (*cur_scope).Register(ptr->GetName(), GetHeap().Make(x, cur_scope));
        } else {
            (*cur_scope).Register(ptr->GetName(), GetHeap().Make(Evaluate(cur_scope, vec[1])));
        }
        return nullptr;
    }
    std::vector<Object*> args;
    ToVector(vec[0], args);
    Object* x(GetHeap().Make(As<Cell>(vec[0])->GetSecond(), As<Cell>(to_eval)->GetSecond()));
    (*cur_scope).Register(As<Symbol>(args[0])->GetName(), GetHeap().Make(x, cur_scope));
    return nullptr;
}

Object* Lambda::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() < 2) {
        throw SyntaxError();
    }
    if (vec[0] != nullptr && !Is<Cell>(vec[0])) {
        throw SyntaxError();
    }
    return GetHeap().Make(to_eval, cur_scope);
}

Object* Variable::Eval(Scope* cur_scope, Object* to_eval) {
    if (auto ptr = dynamic_cast<MyLambda*>(val_)) {
        return ptr->Eval(cur_scope, to_eval);
    }
    return val_;
}

Object* MyLambda::Eval(Scope* cur_scope, Object* to_eval) {
    Scope* new_scope(GetHeap().Make(scope_));
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    Object* cur = nullptr;
    if (Is<Cell>(val_)) {
        cur = As<Cell>(val_)->GetFirst();
    }
    size_t cur_ind = 0;
    while (cur != nullptr) {
        std::string name = As<Symbol>(As<Cell>(cur)->GetFirst())->GetName();
        (*new_scope).Register(name, GetHeap().Make(Evaluate(cur_scope, vec[cur_ind])));
        cur_ind++;
        cur = As<Cell>(cur)->GetSecond();
    }
    if (cur_ind != vec.size()) {
        throw RuntimeError();
    }
    std::vector<Object*> body;
    ToVector(As<Cell>(val_)->GetSecond(), body);
    for (size_t i = 0; i + 1 < body.size(); ++i) {
        Evaluate(new_scope, body[i]);
    }
    return Evaluate(new_scope, body[body.size() - 1]);
}

void MyLambda::Mark() {
    if (mark_) {
        return;
    }
    mark_ = true;
    for (auto ptr : dependencies_) {
        (*ptr).Mark();
    }
    scope_->ScopeMark();
}

Object* Set::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    if (vec.size() != 2) {
        throw SyntaxError();
    }
    if (!Is<Symbol>(vec[0])) {
        throw SyntaxError();
    }
    auto val = Evaluate(cur_scope, vec[1]);
    (*cur_scope).Change(As<Symbol>(vec[0])->GetName(), GetHeap().Make(val));
    return val;
}

Object* SetCar::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    auto val = Evaluate(cur_scope, vec[0]);
    if (!Is<Cell>(val)) {
        throw RuntimeError();
    }
    As<Cell>(val)->ReplaceFirst(Evaluate(cur_scope, vec[1]));
    return nullptr;
}

Object* SetCdr::Eval(Scope* cur_scope, Object* to_eval) {
    std::vector<Object*> vec;
    ToVector(to_eval, vec);
    auto val = Evaluate(cur_scope, vec[0]);
    if (!Is<Cell>(val)) {
        throw RuntimeError();
    }
    As<Cell>(val)->ReplaceSecond(Evaluate(cur_scope, vec[1]));
    return nullptr;
}