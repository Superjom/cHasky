#include "Projection.h"
#include "Repository.h"
using namespace cHasky;

/*
Projection::Projection(const ProjectionConfig& config) : 
    _config(config)
{
    //LOG (INFO) << "init param in Projection";
    // create parameter in repository
    if (_config.has_param_name()) {
        _param = global_repository().create_param(_config.param_name());
    }
}
*/

/*Projection::Projection(Layer& l1, Layer& l2, ProjectionConfig& config) {
    this->connect(l1, l2, config);
}*/
void Projection::connect(costr& l1name, costr& l2name, ProjectionConfig& config) {
    Layer& l1 = * global_repository().get_layer(l1name);
    Layer& l2 = * global_repository().get_layer(l2name);
    connect(l1, l2, config);
}

void Projection::connect(Layer& l1, Layer& l2, ProjectionConfig& config) {
    _config = config;
    // register projection's info
    if (! _config.has_name()) {
        string proj_name = format_string("%s--%s(Proj)", l1.name().c_str(), l2.name().c_str());
        _config.set_name(proj_name);
        string e = format_string("projection between layer [%s] and layer [%s] has been duplicated", 
                l1.name().c_str(), l2.name().c_str());
        CHECK( !global_repository().has_proj(proj_name)) << e;
    }
    if (_config.has_param_name()) {
        _param = global_repository().create_param(_config.param_name());
        init_param(l1, l2);
    }
    // register layer1's output projection
    l1.out_pojs().push_back(this);
    // register layers to projection
    layers().from = &l1;
    layers().to =   &l2;
}


     
