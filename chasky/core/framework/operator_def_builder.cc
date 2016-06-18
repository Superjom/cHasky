#include "chasky/core/framework/operator_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {
OperatorDef& OperatorDefBuilder::Finalize() {
  return def_;
}

}
