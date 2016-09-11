#ifndef CHASKY_CORE_FRAMEWORK_DATA_PROVIDER_H_
#define CHASKY_CORE_FRAMEWORK_DATA_PROVIDER_H_

#include "chasky/core/runtime/postbox.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/data_provider.pb.h"

namespace chasky {

class DataProvider {
public:
  static std::unique_ptr<DataProvider>
  Create(const DataProviderDef &def, PostBox *postbox, EdgeLib *edge_lib);

  // Fill a batch of data to this graph.
  Status Fill(const std::vector<ArgumentDef> &data);

protected:
  DataProvider(const DataProviderDef &def, PostBox *postbox, EdgeLib *edge_lib);

private:
  DataProviderDef def_;
  std::vector<ArgumentPtr> outputs_;
  PostBox *postbox_;
  EdgeLib *edge_lib_;
};

} // namespace chasky
#endif
