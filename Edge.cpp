#include "Edge.h"
using namespace cHasky;

bool Edge::connect(Layer &l1, Layer& l2, ProjectionConfig &proj_config) {
    CHECK(! _projection.get());
    proj_config.set_from_size(l1.config().size());
    proj_config.set_to_size(l2.config().size());
    _projection.reset(new Projection(proj_config));
}
