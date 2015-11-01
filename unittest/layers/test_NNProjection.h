#include "layers/NNProjection.h"
using namespace cHasky;

TEST(NNProjection, init) {
    ProjectionConfig config;
    config.set_param_name("nn1");
    config.set_from_size(10);
    config.set_to_size(20);
    config.set_use_bias(false);
    NNProjection p (config);
}
