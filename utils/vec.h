/*
 * vec.h
 *
 *  Created on: Sep 10, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#pragma once
#include "common.h"
template <typename T>
class Vec {
public:
    typedef T value_type;
    Vec() {
    }
    ~Vec() {
        if(_data != NULL) delete _data;
    }
    Vec& operator= (const Vec& other) {
        if(this != &other) {
            _size = other.size();
            reset(_size);
            for(size_t i = 0; i < _size; i++) {
                data()[i] = other[i];
            }
        }
        return *this;
    }
    Vec(size_t size);
    Vec(const Vec& other);
    Vec(Vec&& other);
    void init(size_t size, bool random_init = false);
    void clear();
    void random();
    void reset(size_t size);
	value_type& operator[](size_t i) {
        CHECK_GE(i, 0);
        CHECK_LE(i, size());
		return _data[i];
	}
	const value_type& operator[](size_t i) const {
        CHECK(i >= 0 && i < size());
		return _data[i];
	}
    void random(float offset=0.);

protected:
    value_type* data() {
        return _data;
    }
    const value_type* data() const {
        return _data;
    }
    size_t size() const {
        return _size;
    }

private:
    value_type *_data = NULL;
    size_t _size = 0;
};

