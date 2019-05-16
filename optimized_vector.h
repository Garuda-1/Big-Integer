#ifndef UNTITLED_OPTIMIZED_VECTOR_HPP
#define UNTIILED_OPTIMIZED_VECTOR_HPP

#include <cstddef>
#include <cstring>
#include "shared_pointer.h"

template<typename T>
class optimized_vector {
    static const size_t _small_object_max_size = 8;

    size_t _size;
    size_t _allocated;

    T _small_object[_small_object_max_size];
    shared_pointer<T> _big_object;


    void convert_to_big_object(size_t to_allocate = _small_object_max_size * 2) {
        _allocated = to_allocate;
        _big_object.reset((T *) operator new[](_allocated * sizeof(T)));
        for (size_t i = 0; i < _size; i++) {
            new(_big_object.get() + i) T(_small_object[i]);
        }
        for (size_t i = 0; i < _size; i++) {
            _small_object[i].~T();
        }
    }

    void extend() {
        T *_new_big_object = (T *) operator new[](_allocated * sizeof(T));
        for (size_t i = 0; i < _size; i++) {
            new(_new_big_object + i) T(_big_object[i]);
        }
        _big_object.reset(_new_big_object);
    }

public:
    optimized_vector() : _size(0), _allocated(0) {}

    optimized_vector(const optimized_vector<T> &that)
            : _size(that._size), _allocated(that._allocated), _big_object(that._big_object) {
        if (that._allocated == 0) {
            for (size_t i = 0; i < that._size; i++) {
                new(_small_object + i) T(that._small_object[i]);
            }
        }
    }

    ~optimized_vector() {
        if (_big_object.get() != nullptr) {
            for (size_t i = 0; i < _size; i++) {
                _big_object[i].~T();
            }
        } else {
            for (size_t i = 0; i < _size; i++) {
                _small_object[i].~T();
            }
        }
    }

    optimized_vector &operator=(const optimized_vector &that) {
        if (that._big_object.get() == nullptr) {
            for (size_t i = 0; i < that.size(); i++) {
                new(_small_object + i) T(that._small_object[i]);
            }
            _big_object.reset();
        } else {
            if (_big_object.get() == nullptr) {
                for (size_t i = 0; i < _size; i++) {
                    _small_object[i].~T();
                }
            }
            _big_object = that._big_object;
        }
        _size = that._size;
        _allocated = that._allocated;

        return *this;
    }

    void push_back(T value) {
        if (_size == _small_object_max_size && _big_object.get() == nullptr) {
            convert_to_big_object();
        }
        if (_big_object.get() == nullptr) {
            new(_small_object + _size) T(value);
        } else {
            if (_allocated == _size) {
                _allocated *= 2;
                extend();
            }
            new(_big_object.get() + _size) T(value);
        }
        ++_size;
    }

    void resize(size_t new_size, const T &value = T()) {
        if (new_size <= _size) {
            _size = new_size;
            if (_big_object.get() != nullptr && _size <= _small_object_max_size) {
                for (size_t i = 0; i < _size; i++) {
                    new(_small_object + i) T(_big_object[i]);
                }
                _allocated = 0;
                _big_object.reset();
            }
        } else {
            if (_small_object_max_size < new_size) {
                if (_big_object.get() == nullptr) {
                    _allocated = _small_object_max_size * 2;
                    while (_allocated < new_size) {
                        _allocated *= 2;
                    }
                    convert_to_big_object(_allocated);
                } else {
                    while (_allocated < new_size) {
                        _allocated *= 2;
                    }
                    extend();
                }
            }
            while (_size < new_size) {
                push_back(value);
            }
        }
    }

    void reserve(size_t to_alloc) {
        if (_small_object_max_size < to_alloc) {
            if (_big_object.get() == nullptr) {
                _allocated = _small_object_max_size * 2;
                while (_allocated < to_alloc) {
                    _allocated *= 2;
                }
                convert_to_big_object(_allocated);
            } else {
                while (_allocated < to_alloc) {
                    _allocated *= 2;
                }
                extend();
            }
        }
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void checkout() {
        if (_big_object.get() != nullptr && !_big_object.independent()) {
            extend();
        }
    }

    T &operator[](size_t i) {
        return (_big_object.get() == nullptr) ? _small_object[i] : _big_object[i];
    }

    const T &operator[](size_t i) const {
        return (_big_object.get() == nullptr) ? _small_object[i] : _big_object[i];
    }
};

#endif
