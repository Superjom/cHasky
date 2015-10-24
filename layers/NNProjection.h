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
    { 
        init_parameter();
    }
    virtual void forward    (Argument& fromag, Argument& toag);
    virtual void backward   (Argument& fromag, Argument& toag);

protected:
    void init_parameter() {
        CHECK(this->config().has_to_size());
        CHECK(this->config().has_from_size());
        this->param().weight.reset(new Mat<T>);
        this->param().bias.reset(new Vec<T>);

        shape_t shape(this->config().to_size(), this->config().from_size());
        this->param().weight->set_shape(shape);
        this->param().bias->init(shape.size);
    }

private:
};  // end class NNProjection
