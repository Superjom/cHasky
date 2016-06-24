#include "chasky/core/framework/operator_def_builder.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/attr_value_util.h"
using namespace std;

namespace chasky {

// Set default width for each output
OperatorDef &OperatorDefBuilder::Finalize() {
  size_t default_width = 0;
  if (def_.inputs_size() > 0) {
    default_width = def_.inputs(0).shape().width();
  }
  for (size_t i = 0; i < def_.outputs_size(); i++) {
    if (def_.outputs(i).shape().width() == 0) {
      def_.mutable_outputs(i)->mutable_shape()->set_width(default_width);
    }
  }
  return def_;
}

}
