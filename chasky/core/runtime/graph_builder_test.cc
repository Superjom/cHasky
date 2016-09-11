#include <gtest/gtest.h>
#include "chasky/core/framework/graph.pb.h"
#include "chasky/core/framework/node_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/runtime/graph_builder.h"
namespace chasky {
namespace test {

class GraphBuilderTest : public ::testing::Test {
public:
  virtual void SetUp() {

  }

  void FillGraphDef() {
    
  }

protected:
  GraphDef graph_def_;
};

} // namespace test
} // chasky
