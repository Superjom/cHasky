#include "Edge.h"
#include "Repository.h"
using namespace cHasky;

bool Edge::connect(Layer &l1, Layer& l2, ProjectionConfig &proj_config) {
    CHECK(! _projection.get());
    proj_config.set_from_size(l1.config().size());
    proj_config.set_to_size(l2.config().size());
    _projection.reset(new Projection(proj_config));
}

bool Edge::connect(const string& l1_name, const string& l2_name, Projection &proj_config) {
    Layer& l1 = global_repository().get_layer(l1_name);
    Layer& l2 = global_repository().get_layer(l2_name);
    connect(l1, l2, proj_config);
}
