#pragma once
/*
 * Repository.h
 *
 *  Created on: Oct 30, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include "utils/all.h"
#include "Layer.h"
#include "proto/LayerConfig.pb.h"
#include "Parameter.h"
#include "Projection.h"

namespace cHasky {


//class Projection;

/*
 * a Repository is the repository of parameters and layers.
 * it control resources, including registration, acquisition, synchronization.  
 */
class Repository {
public:
    typedef std::shared_ptr<Parameter>  param_ptr;
    typedef std::shared_ptr<Layer>      layer_ptr;
    typedef std::shared_ptr<Projection> proj_ptr;

    Parameter*  create_param (const string& name);
    Layer*      create_layer (const string& name, LayerConfig &config);

    param_ptr get_param (const string& name) {
        CHECK(has_param(name));
        return _params[name];
    }
    layer_ptr get_layer (const string& name) {
        CHECK(has_layer(name));
        return _layers[name];
    }
    proj_ptr  get_proj (const string& name) {
        CHECK(has_proj(name));
        return _projs[name];
    }
    bool has_param (const string& name) {
        return _params.count(name) != 0;
    }
    bool has_layer (const string& name) {
        return _layers.count(name) != 0;
    }
    bool has_proj (const string& name) {
        return _projs.count(name) != 0;
    }

protected:
    void register_param (const string& name, param_ptr &&param) {
        CHECK (_params.count(name) == 0) << "[register_param]\tname '" << name << "' had been registered"; 
        _params[name] = param;
    }
    void register_layer (const string& name, layer_ptr &&layer) {
        CHECK (_layers.count(name) == 0) << "[register_layer]\tname '" << name << "' had been registered"; 
        _layers[name] = layer;
    }
    void register_projection (const string& name, proj_ptr &&proj) {
        CHECK (_projs.count(name) == 0) << "[register_projection]\tname '" << name << "' had been registered"; 
        _projs[name] = proj;

    }

private:
    map<string, param_ptr>  _params;
    map<string, layer_ptr>  _layers;
    map<string, proj_ptr>   _projs;
};


//inline Repository& global_repository();
inline Repository& global_repository() {
    static Repository r;
    return r;
}


};  // end namespace cHasky
