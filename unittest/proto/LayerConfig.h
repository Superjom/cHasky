#include "proto/LayerConfig.pb.h"
using namespace cHasky;

TEST(LayerConfig, init) {
    LayerConfig config;
}

TEST(LayerConfig, set) {
    LayerConfig config;
    config.set_name("layer 1");
    ASSERT_TRUE(config.has_name());
    ASSERT_EQ(config.name(), "layer 1");
}

TEST(LayerConfig, clear) {
    LayerConfig config;
    config.set_name("layer 1");
    config.Clear();
    ASSERT_FALSE(config.has_name());
}
