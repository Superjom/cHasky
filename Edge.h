#pragma once
#include "utils/all.h"
#include "proto/LayerConfig.pb.h"
#include "Layer.h"
#include "Projection.h"
/*
 * an Edge is the connection between two Layers.
 * it builds the relation between the layers.
 */
namespace cHasky {


class Edge {
public:
    explicit Edge() { }
    //bool connect(Layer &l1, Layer& l2, Projection &proj);
    // connect layer1 to layer2 according to Projection config
    bool connect(Layer &l1, Layer& l2, ProjectionConfig &proj_config);
    bool connect(const string& l1_name, const string& l2_name, Projection &proj_config);

private:
    std::shared_ptr<Projection> _projection;

};  // end class Edge


};  // end namespace cHasky
