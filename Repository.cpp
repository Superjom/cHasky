#include "Repository.h"
using namespace cHasky;

Parameter* Repository::create_param (const string& name) {
    LOG (INFO) << "create_param:\t[" << name << "]";
    if (has_param(name)) return get_param(name).get();
    param_ptr param(new Parameter);
    register_param (name, std::move(param));
    return param.get();
}
Layer* Repository::create_layer (const string& name, LayerConfig &config) {
    LOG (INFO) << "create layer:\t[" << name << "]";
    if (has_layer(name)) return get_layer(name).get();
    layer_ptr layer (new Layer(config));
    register_layer (name, std::move(layer));
    return layer.get();
}


