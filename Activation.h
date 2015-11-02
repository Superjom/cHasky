#pragma once
#include "utils/all.h"
namespace cHasky {


class Activation {
public:
    //typedef ::google::protobuf::RepeatedField< float > floats_t;
    typedef vector<float> floats_t;
    typedef std::function<void(const floats_t&, floats_t&)> activation_t;
    /*
    enum type {
        sigmoid,
        tanh
    };

    static activation_t get_activation(type t) {
        if (t == sigmoid) return sigmoid;
        return sigmoid;
    }
    */

    static void equal(const floats_t& from, floats_t& to);

    static void diff_equal(const floats_t& from, floats_t& to) {
        if (&from == &to) return;
        CHECK_EQ(from.size(), to.size());
        for (int i = 0; i < from.size(); i ++) {
            //to.Set(i, from.Get(i));
            to[i] = from[i];
        }
    }

    static void sigmoid(const floats_t& from, floats_t& to) {
        CHECK_EQ(from.size(), to.size());
        for( int i = 0; i < from.size(); i++) {
            //float v = from.Get(i);
            float v = from[i];
            //to.Set(i, 1. / (1. + exp(-v)));
            to[i] = 1. / (1. + exp(-v));
        }
    }
    static void diff_sigmoid(const floats_t& from, floats_t& to) {
        CHECK_EQ(from.size(), to.size());
        for( int i = 0; i < from.size(); i ++) {
            //float exp_x = exp(from.Get(i));
            float exp_x = exp(from[i]);
            float v = exp_x / pow((1 + exp_x), 2);
            //to.Set(i, v);
            to[i] = v;
        }
    }
};


};  // end namespace cHasky
