#include "Layer.h"
#include "proto/LayerConfig.pb.h"
#include "layers/NNProjection.h"

using namespace cHasky;

TEST(NNProjection, init) {
    ProjectionConfig config;
    config.set_param_name("nn1");
    config.set_use_bias(false);

    LayerConfig config_l1;
    config_l1.set_name("l1");
    config_l1.set_size(10);
    Layer l1(config_l1);
    

    LayerConfig config_l2;
    config_l2.set_name("l2");
    config_l2.set_size(20);
    Layer l2(config_l2);

    NNProjection p;
    p.connect(l1, l2, config);
}
