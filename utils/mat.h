/*
 * mat.hpp
 *
 *  Created on: Sep 11, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#pragma once
#include "common.h"
#include "vec.h"

template <typename T>
class Mat {
public:
    typedef T value_type;
    typedef Vec<T> vec_t;

    explicit Mat() { }
    explicit Mat(const shape_t &shape) {
        set_shape(shape); 
    }
    explicit Mat(int size, int width) {
        set_shape(shape_t(size, width)); 
    }

    void set_shape(const shape_t& shape);

    const shape_t& shape() const;
    
    vec_t& operator[] (int id) {
        CHECK_GE(id, 0);
        CHECK_LT(id, _shape.size);
        return _data[id];
    }
    const vec_t& operator[] (int id) const {
        CHECK_GE(id, 0);
        CHECK_LT(id, _shape.size);
        return _data[id];
    }

private:
    shape_t _shape;
    vector<Vec<value_type> > _data;
};
