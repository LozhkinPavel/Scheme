#pragma once

#include <memory>
#include <vector>

class Object : public std::enable_shared_from_this<Object> {
    friend class Heap;

public:
    virtual ~Object() = default;
    virtual void Mark() {
        if (mark_) {
            return;
        }
        mark_ = true;
        for (auto ptr : dependencies_) {
            (*ptr).Mark();
        }
    }

protected:
    void AddDependency(Object* obj) {
        dependencies_.push_back(obj);
    }
    void RemoveDependency(Object* obj) {
        for (size_t i = 0; i < dependencies_.size(); ++i) {
            if (dependencies_[i] == obj) {
                dependencies_.erase(dependencies_.begin() + i);
                return;
            }
        }
    }

protected:
    std::vector<Object*> dependencies_;
    bool mark_ = false;
};

class CloseBracket : public Object {
    friend class Heap;
};

class Dot : public Object {
    friend class Heap;
};

class Number : public Object {
    friend class Heap;

public:
    int GetValue() const {
        return value_;
    }

private:
    Number(int value) : value_(value) {
    }
    int value_ = 0;
};

class Boolean : public Object {
    friend class Heap;

public:
    int GetFlag() const {
        return flag_;
    }

private:
    Boolean(bool flag) : flag_(flag) {
    }
    bool flag_ = 0;
};

class Symbol : public Object {
    friend class Heap;

public:
    const std::string& GetName() const {
        return name_;
    }

private:
    Symbol(std::string name) : name_(name) {
    }
    std::string name_;
};

class Cell : public Object {
    friend class Heap;

public:
    Object* const& GetFirst() const {
        return first_;
    }
    Object* const& GetSecond() const {
        return second_;
    }

    Object*& GetFirst() {
        return first_;
    }
    Object*& GetSecond() {
        return second_;
    }

    void ReplaceSecond(Object* obj) {
        RemoveDependency(second_);
        second_ = obj;
        AddDependency(obj);
    }

    void ReplaceFirst(Object* obj) {
        RemoveDependency(first_);
        first_ = obj;
        AddDependency(obj);
    }

private:
    Cell(Object* first, Object* second) : first_(first), second_(second) {
    }
    Object* first_;
    Object* second_;
};

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return (dynamic_cast<T*>(obj.get()) != nullptr);
}

template <class T>
T* As(Object* obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
bool Is(Object* obj) {
    return (dynamic_cast<T*>(obj) != nullptr);
}
