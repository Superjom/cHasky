#pragma once
/*
 * Paramter is model's paramters like weight matrix of NN.
 */
#include "utils/all.h"

namespace cHasky {


struct Parameter {

// weight matrix
std::shared_ptr<Mat> weight_p;
std::shared_ptr<Vec> bias_p;

Mat& weight() {
    CHECK(weight_p.get());
    return *weight_p;
}

Vec& bias() {
    CHECK(bias_p.get());
    return *bias_p;
}

};  // end struct Paramter


};  // end namespace cHasky
