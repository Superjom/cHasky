#pragma once
#include "utils/all.h"
#include "proto/Argument.pb.h"
#include "proto/ProjectionConfig.pb.h"
#include "Parameter.h"
using namespace cHasky;
/*
 * 
 * A projection is an ensemble of connections between a subset of layers.
 * It may have paramters sometimes, weight matrix in 
 *  full-connection forward-backward NN for example.
 */
template <typename T>
class Projection {
public:
    // all the subchild of Projection should call base Projection's constructor
    Projection(ProjectionConfig& config) : 
        _config(config) 
    { }
    // re-initialize all the parameters of this projection
    virtual void reset() { };
    // free all parameter's memory
    virtual void clear() { };
    virtual void init(ProjectionConfig& config) { };
    virtual void forward    (Argument& fromag, Argument& toag) { };
    virtual void backward   (Argument& fromag, Argument& toag) { };

    ProjectionConfig& config() {
        return _config;
    }
    Parameter<T>& param() {
        return _param;
    }
    // get configs
    const string& name() {
        return config().get_name();
    }

private:
    ProjectionConfig _config;
    Parameter<T> _param;
};  // end class Projection

