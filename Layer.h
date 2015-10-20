#pragma once
/*
 * Layer.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Superjom
 *      Email: yanchunwei@outlook.com
 */
#include "utils/all.h"
#include "proto/LayerConfig.pb.h"
using namespace cHasky;

template <typename T>
class Layer {
public:
    const LayerConfig& config() const {
        return _config;
    }

private:
    LayerConfig _config;
};  // end class Layer
