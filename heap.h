#pragma once

#include <unordered_map>
#include "object.h"
#include "functions.h"

template <class T, class U>
concept Derived = std::is_base_of<U, T>::value || std::is_same_v<Scope, T>;

class Heap {
public:
    ~Heap();
    template <Derived<Object> T, typename... Args>
    T* Make(Args... args) {
        auto ptr = new T(args...);
        objects_.insert(std::make_pair(ptr, true));
        return ptr;
    }

    Cell* Make(Object* first, Object* second);
    MyLambda* Make(Object* val, Scope* ptr1);
    Variable* Make(Object* val);
    Scope* Make();
    Scope* Make(Scope* rhs);
    void Delete();
    void Clear();

private:
    std::unordered_map<Object*, bool> objects_;
    std::unordered_map<Scope*, bool> scopes_;
};

Heap& GetHeap();
