#ifndef CHASKY_CORE_FRAMEWORK_DATA_PROVIDER_DEF_BUILDER_H_
#define CHASKY_CORE_FRAMEWORK_DATA_PROVIDER_DEF_BUILDER_H_

#include "chasky/core/framework/argument.pb.h"
#include "chasky/core/framework/data_provider.pb.h"

namespace chasky {

class DataProviderDefBuilder {
public:
  explicit DataProviderDefBuilder() {}

  DataProviderDefBuilder &Name(const std::string &x) {
    def_.set_name(x);
    return *this;
  }

  DataProviderDefBuilder &Field(const ArgumentDef &x) {
    *def_.add_outputs() = x;
    return *this;
  }

  DataProviderDef &Finalize() { return def_; }

private:
  DataProviderDef def_;
};

} // namespace chasky
#endif
