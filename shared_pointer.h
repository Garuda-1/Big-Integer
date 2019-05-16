#ifndef UNTITLED_SHARED_POINTER_H
#define UNTITLED_SHARED_POINTER_H

#include <cstddef>

template<typename T>
struct shared_pointer {
    struct Aux {
        explicit Aux(T *ptr) {
            ptr_ = ptr;
            cnt_ = 1;
        }

        ~Aux() {
            delete (ptr_);
        }

        T *ptr_;
        int cnt_;
    };

    explicit shared_pointer(T *ptr = nullptr) {
        if (ptr == 0) {
            aux = nullptr;
        } else {
            aux = new Aux(ptr);
        }
    }

    ~shared_pointer() {
        if (aux != nullptr) {
            --aux->cnt_;
            if (aux->cnt_ == 0) {
                delete (aux);
            }
        }
    }

    shared_pointer(const shared_pointer &a) {
        if (a.aux == nullptr) {
            aux = nullptr;
        } else {
            aux = a.aux;
            ++aux->cnt_;
        }
    }

    shared_pointer &operator=(const shared_pointer &a) {
        if (aux != nullptr && a.aux != nullptr && aux == a.aux) {
            return *this;
        }
        if (aux != nullptr) {
            --aux->cnt_;
            if (aux->cnt_ == 0) {
                delete (aux);
            }
        }
        if (a.aux == nullptr) {
            aux = nullptr;
        } else {
            aux = a.aux;
            ++aux->cnt_;
        }

        return *this;
    }

    T *get() const {
        return (aux == nullptr) ? nullptr : aux->ptr_;
    }

    void reset(T *ptr = nullptr) {
        if (aux != nullptr) {
            --aux->cnt_;
            if (aux->cnt_ == 0) {
                delete (aux);
            }
        }
        if (ptr == nullptr) {
            aux = nullptr;
        } else {
            aux = new Aux(ptr);
        }
    }

    T &operator*() const {
        return *(aux->ptr_);
    }

    T *operator->() const {
        return (aux == nullptr) ? nullptr : aux->ptr_;
    }

    T &operator[](size_t i) {
        return aux->ptr_[i];
    }

private:
    Aux *aux;
};

#endif


