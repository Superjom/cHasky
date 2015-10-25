#include "layers/NNProjection.h"
using namespace cHasky;

void NNProjection::forward   (Argument& fromag, Argument& toag) {
    CHECK_EQ( this->param().weight().shape().width, fromag.activation_size());
    CHECK_EQ( this->param().weight().shape().size, toag.activation_size());
    CHECK_EQ( this->param().bias().size(), toag.activation_size());
    val_t v = 0; 
    for (int i=0; i<toag.activation_size(); i++) {
        v = toag.activation(i);
        for (int j=0; j<fromag.activation_size(); j++) {
            v += fromag.activation(j) * this->param().weight()[i][j];
        }
        CHECK(this->config().has_use_bias());
        if (this->config().use_bias()) {
            v += this->param().bias()[i];
        }
        toag.set_activation(i, v);
    }
}

void NNProjection::backward   (Argument& fromag, Argument& toag) {
    cout << "hello world";
}
