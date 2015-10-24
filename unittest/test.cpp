// utils
#include "utils/vec.h"
//#include "proto/LayerConfig.h"
//#include "layers/test_NNProjection.h"

TEST(Vec, init) {
    Vec<float> v;
}

int main(int argc, char **argv) {  
    //GOOGLE_PROTOBUF_VERIFY_VERSION;

    testing::InitGoogleTest(&argc, argv);  
    // Optional:  Delete all global objects allocated by libprotobuf.
    //google::protobuf::ShutdownProtobufLibrary();
    return RUN_ALL_TESTS();  
} 

