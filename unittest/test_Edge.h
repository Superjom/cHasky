#include "Edge.h"
#include "Layer.h"
using namespace cHasky;

TEST(Edge, init) {
    Edge e;
    LayerConfig layer_config;
    layer_config.set_size(10);
    layer_config.set_name("l1");
    Layer l1(layer_config);
    Layer l2(layer_config);
    ProjectionConfig proj_config;

    e.connect(l1, l2, proj_config);
}
