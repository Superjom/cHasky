#pragma once
#include "utils/all.h"
#include "proto/LayerConfig.pb.h"
#include "Layer.h"
#include "Projection.h"
using namespace cHasky;

/*
 * an Edge is the connection between two Layers.
 * it builds the relation between the layers.
 */
template<typename T>
class Edge {
public:
    explicit Edge() { }
    //bool connect(Layer &l1, Layer& l2, Projection &proj);
    // connect layer1 to layer2 according to Projection config
    bool connect(Layer &l1, Layer& l2, const ProjectionConfig &proj_config);
};  // end class Edge
