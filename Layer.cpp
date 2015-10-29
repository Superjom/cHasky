#include "Layer.h"
using namespace cHasky;

Layer::activation_t Layer::get_activation() {
    if (config().activate_config().type() == ActivateConfig_Type_EQUAL) 
        return Activation::equal;
    if (config().activate_config().type() == ActivateConfig_Type_SIGMOID) 
        return Activation::sigmoid;
}

Layer::activation_t Layer::get_diff_activation() {
    if (config().activate_config().type() == ActivateConfig_Type_EQUAL) 
        return Activation::diff_equal;
    if (config().activate_config().type() == ActivateConfig_Type_SIGMOID) 
        return Activation::diff_sigmoid;
}
