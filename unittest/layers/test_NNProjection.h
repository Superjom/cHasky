#include "layers/NNProjection.h"

TEST(NNProjection, init) {
    ProjectionConfig config;
    NNProjection<float> p (config);
}
