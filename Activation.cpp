#include "Activation.h"
using namespace cHasky;

void Activation::equal(const floats_t& from, floats_t& to) {
    if (&from == &to) return;
    CHECK_EQ(from.size(), to.size());
    for (int i = 0; i < from.size(); i ++) {
        //to.Set(i, from.Get(i));
        to[i] = from[i];
    }
}
