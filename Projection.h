#pragma once
#include "utils/all.h"
#include "proto/Argument.pb.h"
#include "proto/ProjectionConfig.pb.h"
#include "Parameter.h"
/*
 * A projection is an ensemble of connections between a subset of layers.
 * It may have paramters sometimes, weight matrix in 
 *  full-connection forward-backward NN for example.
 */
namespace cHasky {


class Layer;

class Projection {
public:
    struct ConnectedLayers {
        Layer* from;
        Layer* to;
    };

    // all the subchild of Projection should call base Projection's constructor
    //Projection(const ProjectionConfig& config);
    //Projection(Layer& l1, Layer& l2, ProjectionConfig& config);
    void connect(costr& l1name, costr& l2name, ProjectionConfig& config);
    virtual void connect(Layer& l1, Layer& l2, ProjectionConfig& config);
    virtual void init_param(Layer& l1, Layer& l2);
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
        return *_param;
    }
    // get configs
    const string& name() {
        return config().name();
    }

    ConnectedLayers& layers() {
        return _layers;
    }

private:
    ProjectionConfig    _config;
    Parameter*          _param;
    // the starting point and the end point of a projection
    ConnectedLayers     _layers;
};  // end class Projection


};  // end namespace cHasky
