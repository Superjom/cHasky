#pragma once
/*
 * common.h
 *
 *  Created on: Sep 10, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <map>
#include <thread>
#include <memory>
using namespace std;
#include "glog/logging.h"
#include "gtest/gtest.h"
#include "gflags/gflags.h"
struct shape_t;

typedef unsigned int            uint;
typedef vector<shape_t>         vshape_t;
typedef const vector<shape_t>   cvshape_t;

const float EPISILON = 1e-4;

enum layer_kind_t {
    HIDDEN_LAYER,
    INPUT_LAYER,
    OUTPUT_LAYER
};

struct shape_t {
    uint size = -1;
    uint width = -1;

    shape_t() { }
    shape_t(int size, int width) {
        CHECK_GT(size, 0);
        CHECK_GT(width, 0);
        this->size = size;
        this->width = width;
    }

    friend ostream& operator<< (ostream& os, const shape_t &shape) {
        os << shape.size << " " << shape.width;
        return os;
    }
};

