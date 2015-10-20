// utils
#include "utils/vec.h"
#include "proto/LayerConfig.h"
#include "layers/test_NNProjection.h"

TEST(Vec, init) {
    Vec<float> v;
}

int main(int argc, char **argv) {  

    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();  
} 

