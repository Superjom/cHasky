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
    CHECK(outputs_[i]);
    DLOG(INFO) << "fill " << i << "th argument";
    outputs_[i]->DeSerialize(data[i]);
    // release argument to postbox
    auto key = PostBox::CreateArgKey(def_.name(), def_.outputs(i).name());
    DLOG(INFO) << "register data to postbox " << outputs_[i]->Description();
    CH_CHECK_OK(postbox_->Send(key, outputs_[i]));
  }

  return status;
}

DataProvider::DataProvider(const DataProviderDef &def, PostBox *postbox,
                           EdgeLib *edge_lib)
    : postbox_(postbox), edge_lib_(edge_lib), def_(def) {
  CHECK(!def_.name().empty()) << "should set data provider's name";
  CHECK(outputs_.empty());
  CHECK(postbox_);
  CHECK(edge_lib_);

  for (const auto &arg_def : def.outputs()) {
    outputs_.emplace_back(std::make_shared<Argument>(&arg_def));
    auto key = PostBox::CreateArgKey(def_.name(), arg_def.name());
    CH_CHECK_OK(postbox_->Register(key, outputs_.back()));
  }
}

} // namespace chasky
