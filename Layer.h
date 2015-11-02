#pragma once
/*
 * Layer.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include "utils/all.h"
#include "proto/LayerConfig.pb.h"
#include "Activation.h"
namespace cHasky {

class Projection;

class Layer {
public:
    typedef Activation::activation_t activation_t;

    Layer (const LayerConfig &config) : 
        _config(config) 
    { }
    const LayerConfig& config() const {
        return _config;
    }
    activation_t get_activation();
    activation_t get_diff_activation();

    vector<Projection*>& out_pojs() {
        return _out_pojs;
    }

    const string& name() {
        CHECK(_config.has_name());
        return _config.name();
    }

private:
    // out projections projects from this layers to other layers
    vector<Projection*> _out_pojs;

    LayerConfig _config;
    // the times to be referenced as the left side(start point) of an Edge
    int _ref_counter = 0;
    // arrived by other layers which point to this layer
    int _finish_counter = 0;
};  // end class Layer


};  // end namespace cHasky
