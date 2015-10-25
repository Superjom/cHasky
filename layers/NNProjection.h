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
/*
 * a NNProjection is the implementation of the multi-layer neural networks' projection
 * the projection is 
 *  y_k = b_k + W_k y_{k-1}
 *
 * refer to Chapter 1 from Hinton's <<Deep Learning>> for detail.
 */
namespace cHasky {


class NNProjection : public Projection {
public:

    NNProjection(ProjectionConfig& config) :
        Projection(config) 
    { 
        init_parameter();
    }
    virtual void forward    (Argument& fromag, Argument& toag);
    virtual void backward   (Argument& fromag, Argument& toag);

protected:
    void init_parameter() {
        CHECK(this->config().has_to_size());
        CHECK(this->config().has_from_size());
        this->param().weight_p.reset(new Mat);
        this->param().bias_p.reset(new Vec);

        shape_t shape(this->config().to_size(), this->config().from_size());
        this->param().weight().set_shape(shape);
        this->param().bias().init(shape.size);
    }

private:
};  // end class NNProjection

};  // end namespace cHasky
