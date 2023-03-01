#include "heap.h"
#include <vector>

Heap& GetHeap() {
    static Heap heap;
    return heap;
}

Heap::~Heap() {
    std::vector<Object*> to_delete;
    for (auto p : objects_) {
        to_delete.push_back(p.first);
    }
    for (auto ptr : to_delete) {
        objects_.erase(ptr);
        delete ptr;
    }

    std::vector<Scope*> to_delete_sc;
    for (auto p : scopes_) {
        to_delete_sc.push_back(p.first);
    }
    for (auto ptr : to_delete_sc) {
        scopes_.erase(ptr);
        delete ptr;
    }
}

void Heap::Delete() {
    std::vector<Object*> to_delete;
    for (auto p : objects_) {
        if (!p.first->mark_) {
            to_delete.push_back(p.first);
        } else {
            p.first->mark_ = false;
        }
    }
    for (auto ptr : to_delete) {
        objects_.erase(ptr);
        delete ptr;
    }

    std::vector<Scope*> to_delete_sc;
    for (auto p : scopes_) {
        if (!p.first->mark_) {
            to_delete_sc.push_back(p.first);
        } else {
            p.first->mark_ = false;
        }
    }
    for (auto ptr : to_delete_sc) {
        scopes_.erase(ptr);
        delete ptr;
    }
}

void Heap::Clear() {
    std::vector<Object*> to_delete;
    for (auto p : objects_) {
        to_delete.push_back(p.first);
    }
    for (auto ptr : to_delete) {
        objects_.erase(ptr);
        delete ptr;
    }

    std::vector<Scope*> to_delete_sc;
    for (auto p : scopes_) {
        to_delete_sc.push_back(p.first);
    }
    for (auto ptr : to_delete_sc) {
        scopes_.erase(ptr);
        delete ptr;
    }
}

Scope* Heap::Make() {
    auto ptr = new Scope();
    scopes_.insert(std::make_pair(ptr, true));
    return ptr;
}

Cell* Heap::Make(Object* first, Object* second) {
    auto ptr = new Cell(first, second);
    objects_.insert(std::make_pair(ptr, true));
    if (first != nullptr) {
        ptr->AddDependency(first);
    }
    if (second != nullptr) {
        ptr->AddDependency(second);
    }
    return ptr;
}

MyLambda* Heap::Make(Object* val, Scope* ptr1) {
    auto ptr = new MyLambda(val, ptr1);
    objects_.insert(std::make_pair(ptr, true));
    if (val != nullptr) {
        ptr->AddDependency(val);
    }
    return ptr;
}

Scope* Heap::Make(Scope* rhs) {
    auto ptr = new Scope(rhs);
    scopes_.insert(std::make_pair(ptr, true));
    return ptr;
}

Variable* Heap::Make(Object* val) {
    auto ptr = new Variable(val);
    objects_.insert(std::make_pair(ptr, true));
    if (val != nullptr) {
        ptr->AddDependency(val);
    }
    return ptr;
}