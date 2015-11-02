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

    /*NNProjection(ProjectionConfig& config) :
        Projection(config) 
    { 
        CHECK(this->config().has_param_name()) << "NNProjection should have a param_name to init parameter";
        init_parameter();
    } */
    //virtual void connect(Layer& l1, Layer& l2, ProjectionConfig& config);
    virtual void forward    (Argument& fromag, Argument& toag);
    virtual void backward   (Argument& fromag, Argument& toag);

protected:
    virtual void init_param(Layer& l1, Layer& l2);

private:
};  // end class NNProjection

};  // end namespace cHasky
