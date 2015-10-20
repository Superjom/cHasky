#pragma once
/*
 * NNProjection.h
 *
 *  Created on: Oct 19, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include "../utils/all.h"
#include "../Projection.h"
using namespace cHasky;
/*
 * a NNProjection is the implementation of the multi-layer neural networks' projection
 * the projection is 
 *  y_k = b_k + W_k y_{k-1}
 *
 * refer to Chapter 1 from Hinton's <<Deep Learning>> for detail.
 */
template <typename T>
class NNProjection : public Projection<T> {
public:
    NNProjection(ProjectionConfig& config) :
        Projection<T>(config) 
    { }

private:
};  // end class NNProjection
