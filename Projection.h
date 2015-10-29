#pragma once
#include "utils/all.h"
#include "proto/Argument.pb.h"
#include "proto/ProjectionConfig.pb.h"
#include "Parameter.h"
/*
 * 
 * A projection is an ensemble of connections between a subset of layers.
 * It may have paramters sometimes, weight matrix in 
 *  full-connection forward-backward NN for example.
 */
namespace cHasky {


class Projection {
public:
    // all the subchild of Projection should call base Projection's constructor
    Projection(const ProjectionConfig& config) : 
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
    Parameter& param() {
        return _param;
    }
    // get configs
    const string& name() {
        return config().name();
    }

private:
    ProjectionConfig _config;
    Parameter _param;
};  // end class Projection


};  // end namespace cHasky
