#include "vec.h"
using namespace cHasky;

Vec::Vec(size_t size) {
    init(size);
}

Vec::Vec(const Vec& other) {
    if(_size != other.size()) {
        if(_data) {
            delete _data;
            _data = NULL;
            _size = 0;
        }
        init(other.size());
    }
    memcpy(data(), other.data(), sizeof(value_type) * size());
}

Vec::Vec(Vec&& other) {
    if(_data != NULL) {
        delete _data;
        _data = NULL;
    }
    _data = other._data;
    _size = other._size;
    other._data = NULL;
    other._size = 0;
}
/*
template<typename T>
void Vec<T>::init(size_t size, bool random_init) {
    CHECK_GT(size, 0);
    CHECK_EQ(_data, static_cast<value_type*>(NULL)) << "data can be inited only once";
    //_data.reset(new value_type[size]);
    reset(size);
    for(size_t i = 0; i < _size; ++i) {
        data()[i] = 0.0;
    }
    if(random_init) random(0.0);
}
*/

void Vec::clear() {
    for (int i = 0; i < _size; i++) {
        _data[i] = 0;
    }
}

void Vec::random(float offset) {
    for(size_t i = 0; i < size(); i++)
        _data[i] = (rand() / (float) RAND_MAX - offset) / _size;
}

void Vec::reset (size_t size_) {
    CHECK_GT(size_ , 0);
    if (size_ == _size) return;
    if(_data != NULL) {
        delete _data;
        _size = 0;
    }
    _size = size_;
    _data = new value_type[size_];
}
