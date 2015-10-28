#include "layers/NNProjection.h"
using namespace cHasky;
/*
 * Project from fromag's output to toarg's output
 */
void NNProjection::forward   (Argument& fromag, Argument& toag) {
    CHECK_EQ( this->param().weight().shape().width, fromag.output_size());
    CHECK_EQ( this->param().weight().shape().size, toag.output_size());
    CHECK_EQ( this->param().bias().size(), toag.output_size());
    val_t v = 0; 
    for (int i=0; i<toag.output_size(); i++) {
        //v = toag.output(i);
        v = 0;
        for (int j=0; j<fromag.output_size(); j++) {
            v += fromag.output(j) * this->param().weight()[i][j];
        }
        CHECK(this->config().has_use_bias());
        if (this->config().use_bias()) {
            v += this->param().bias()[i];
        }
        toag.set_output(i, v);
    }
}

/*
 */
void NNProjection::backward   (Argument& fromag, Argument& toag) {
    CHECK_EQ( this->param().weight().shape().width, fromag.output_size());
    CHECK_EQ( this->param().weight().shape().size, toag.output_size());
    CHECK_EQ( this->param().bias().size(), toag.output_size());
    float v = 0;
    // update toag's loss
    for(int i = 0; i < fromag.loss_size(); i++) {
        for (int j = 0; j < toag.loss_size(); j++) {
            v = this->param().weight()[j][i] * fromag.loss(j);
            toag.set_loss(i, v);
        }
    }
    // update weight
    for (int i = 0; i < toag.loss_size(); i++) {
        auto& weight = this->param().weight();
        for (int j = 0; j < fromag.loss_size(); j++) {
            weight[j][i] -= this->config().learning_rate() * \
                        fromag.loss(j) * toag.output(i);
        }
    }


}
