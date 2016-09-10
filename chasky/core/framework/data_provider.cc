#include <memory>

#include "chasky/core/framework/data_provider.h"

namespace chasky {

std::unique_ptr<DataProvider> DataProvider::Create(const DataProviderDef &def,
                                                   PostBox *postbox,
                                                   EdgeLib *edge_lib) {
  return std::unique_ptr<DataProvider>(
      new DataProvider(def, postbox, edge_lib));
}

Status DataProvider::Fill(const std::vector<ArgumentDef> &data) {
  Status status;

  CHECK_EQ(outputs_.size(), data.size());

  for (size_t i = 0; i < outputs_.size(); i++) {
    outputs_[i].DeSerialize(data[i]);
  }

  return status;
}

DataProvider::DataProvider(const DataProviderDef &def, PostBox *postbox,
                           EdgeLib *edge_lib)
    : postbox_(postbox), edge_lib_(edge_lib) {
  CHECK(outputs_.empty());
  CHECK(postbox_);
  CHECK(edge_lib_);

  for (const auto &arg_def : def.outputs()) {
    outputs_.emplace_back(&arg_def);
    CH_CHECK_OK(postbox_->Register(arg_def.name(), &outputs_.back()));
  }
}

} // namespace chasky
