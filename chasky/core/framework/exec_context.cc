#include "chasky/core/framework/exec_context.h"

namespace chasky {
// TODO Add more check
bool ExecContext::IsValid() const { return persistent_argument_ != nullptr; }

Status ExecContext::CreateFromDef(const chasky::OperatorDef &def) {
  // TODO prepare inputs
  // prepare outputs, allocate memory and hold it
  return Status();
}

Status ExecContext::AddInput(const ArgumentDef &def) {
  input_args_.emplace_back(&def);
  return Status();
}

Status ExecContext::AddOutpuut(const ArgumentDef &def) {
  output_args_.emplace_back(&def);
  return Status();
}
}
