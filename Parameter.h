#pragma once
/*
 * Paramter is model's paramters like weight matrix of NN.
 */
#include "utils/all.h"

template <typename T>
struct Parameter {

// weight matrix
std::shared_ptr<Mat<T>> weight;

std::shared_ptr<Vec<T>> bias;

};  // end struct Paramter
